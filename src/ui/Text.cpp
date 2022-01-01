#include "ui/Text.h"


#include "core/InputListener.h"
#include "graphics/Font.h"
#include "graphics/rData.h"
#include "utils/Utils.h"

namespace MidiFi
{
    namespace UI
    {
        static ShiftedKey USStandardEncodingShiftedKeys[21] = 
        {
            { GLFW_KEY_APOSTROPHE,    '"' },
            { GLFW_KEY_COMMA,         '<' },
            { GLFW_KEY_MINUS,         '_' },
            { GLFW_KEY_PERIOD,        '>' },
            { GLFW_KEY_SLASH,         '?' },
            { GLFW_KEY_0,             ')' },
            { GLFW_KEY_1,             '!' },
            { GLFW_KEY_2,             '@' },
            { GLFW_KEY_3,             '#' },
            { GLFW_KEY_4,             '$' },
            { GLFW_KEY_5,             '%' },
            { GLFW_KEY_6,             '^' },
            { GLFW_KEY_7,             '&' },
            { GLFW_KEY_8,             '*' },
            { GLFW_KEY_9,             '(' },
            { GLFW_KEY_SEMICOLON,     ':' },
            { GLFW_KEY_EQUAL,         '+' },
            { GLFW_KEY_LEFT_BRACKET,  '{' },
            { GLFW_KEY_BACKSLASH,     '|' },
            { GLFW_KEY_RIGHT_BRACKET, '}' },
            { GLFW_KEY_GRAVE_ACCENT,  '~' }
        };

        ShiftedKey *currentEncoding = USStandardEncodingShiftedKeys;

        void setCurrentEncoding(ShiftedKey *encoding)
        {
            // other checks should be made, but i can't think of any
            currentEncoding = encoding;
        }

        char getShiftedKey(int GLFWKey)
        {
            // you shouldn't really put a-z into this function, but in case you did... 
            if (GLFWKey >= 'a' && GLFWKey <= 'z')
            {
                return (char) GLFWKey - 32;
            }

            // TODO: binary search?
            for (int i = 0; i < 21; i++) // 21 non-qwerty keys
            {
                if (currentEncoding[i].GLFWKey == GLFWKey)
                {
                    return currentEncoding[i].shiftedChar;
                }
            }

            __pWarning("Tried to get corresponding shifted key of %c, which doesn't have one.", (char) GLFWKey);
            return ' ';
        }

        bool isFunctionKey(int key)
        {
            // by glfw specification, if a key has a code > 255, it's a function key.
            return key > 255;
        }

        void Text::init(glm::vec3 pos, glm::vec4 color, float width, float height, std::string text, Graphics::Font &f)
        {
            this->pos = pos;
            this->color = color;
            this->width = width;
            this->height = height;
            this->text = text;
            this->font = &f;
        }

        int Text::toRData(Graphics::rData &r, unsigned int rOffset)
        {
            using namespace MidiFi::Graphics;

            __pAssert(!((rOffset + this->text.size()) * 4 >= r.vertCount), "rData not big enough to hold UI data!");
            __pAssert(this->font != nullptr, "Text must be initialized with a font.");

            int stride = rOffset * getLayoutLen(r) * 4;

            // for each character in the text, add a default quad
            glm::vec3 posAccumulate = {0.0f, 0.0f, 0.0f};
            const float scale = this->font->fontSize / (this->font->ascent - this->font->descent);
            
            for (int i = 0; i < this->text.length(); i++)
            {
                char c = this->text[i];
                Glyph g = getGlyph(*this->font, c);

                ////////          ////////
                // Insertion into rData //
                ////////          ////////

                // kern depending on the previous char

                // kern please
                if (i != 0)
                {
                    posAccumulate += screenToWorldSize({scale * stbtt_GetCodepointKernAdvance(&(g.parent->info), this->text[i - 1], this->text[i]), 0.0f});
                }

                for (int j = 0; j < 4; j++)
                {
                    glm::vec3 orientation;

                    switch (j)
                    {
                        case 0: orientation = {1.0f * g.width, 1.0f * g.height, 0.0f}; break;
                        case 1: orientation = {0.0f * g.width, 1.0f * g.height, 0.0f}; break;
                        case 2: orientation = {0.0f * g.width, 0.0f * g.height, 0.0f}; break;
                        case 3: orientation = {1.0f * g.width, 0.0f * g.height, 0.0f}; break;
                    }

                    off_len result = getAttribMetaData(r, PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        // instead of position by bottom corner
                        this->pos += orientation - glm::vec3{0.0f, g.height, 0.0f} + posAccumulate + glm::vec3{0.0f, g.descent, 0.0f};

                        // TODO: just use memcpy, bonehead.
                        for (int k = 0; k < 3; k++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&this->pos)[k];
                        }

                        this->pos -= orientation - glm::vec3{0.0f, g.height, 0.0f} + posAccumulate + glm::vec3{0.0f, g.descent, 0.0f};
                    }
                    
                    result = getAttribMetaData(r, PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&this->color)[k];
                        }             
                    }

                    result = getAttribMetaData(r, PONT_TEXCOORD);
                    if (result.second >= 2 * sizeof(float))
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&g.texCoords)[j * 2 + k];
                        }
                    }

                    result = getAttribMetaData(r, PONT_TEXID);
                    if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                    {
                        if (this->font == nullptr)
                        {
                            *(float *)((char *)r.data + result.first + stride) = 0.0f;
                        }
                        else
                        {
                            *(float *)((char *)r.data + result.first + stride) = this->font->texID;
                        }
                    }
                    stride += getLayoutLen(r);
                }

                ////////                                   ////////
                // Spacing of chars based on Text specifications //
                ////////                                   ////////

                // if char is a space, check if total width of next word causes overflow.
                int nextWordLength = 0;
                if (c == ' ')
                {
                    if (i + 1 != this->text.length())
                    {
                        int next = i + 1;
                        char curr = '\0';
                        while (curr != ' ' && next < this->text.length())
                        {
                            curr = this->text.at(next);
                            nextWordLength += getGlyph(*this->font, curr).width;
                            next++;
                        }
                    }
                }

                posAccumulate.x += g.width;

                // if align is to the left
                if (nextWordLength + posAccumulate.x > this->width && i + 1 < this->text.length() && this->text[i + 1] != ' ')
                {
                    posAccumulate.x = 0;
                    posAccumulate.y += screenToWorldSize(glm::vec2(0.0f, g.parent->ascent - g.parent->descent)).y;
                }

            }
            
            __pAssert(scale == this->font->fontSize / (this->font->ascent - this->font->descent), "A font was trampled on!");

            return stride / (4 * getLayoutLen(r));
        }

        void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property)
        {
            
        }

        void Text::setFocus(Text &t)
        {
            static unsigned int callbackHandle = 0;
            // if I include t in the capture list it becomes a lambda expression, which 
            // cannot be casted to a function pointer, so I use this updated static var
            // Also, using a Text& is kinda tricky, and I need a reference to t, so I'll 
            // use a pointer
            static Text *forCapture;
            if (forCapture != nullptr)
                forCapture->focus = false; // I really should use a state machine after this
            forCapture = &t; 

            void (* cb)(int key, int action) = [](int key, int action)
            {
                if (action == GLFW_PRESS || action == GLFW_REPEAT)
                {
                    if (key == GLFW_KEY_BACKSPACE)
                    {
                        forCapture->text.pop_back();
                    }
                    else if (!isFunctionKey(key))
                    {
                        // handle qwerty keys
                        if (key >= 'A' && key <= 'Z')
                        {
                            if (IO::isKeyPressed(GLFW_KEY_LEFT_SHIFT) || IO::isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
                            {
                                forCapture->text.push_back((char) key);
                            }
                            else
                            {
                                forCapture->text.push_back((char) key + 32);
                            }
                        }
                        else
                        {
                            if (IO::isKeyPressed(GLFW_KEY_LEFT_SHIFT) || IO::isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
                            {
                                forCapture->text.push_back((char) getShiftedKey(key));
                            }
                            else
                            {
                                forCapture->text.push_back((char) key);
                            }
                        }
                    }
                }
            };

            if (callbackHandle == 0)
            {
                callbackHandle = IO::submitKeyCallback(cb);
                forCapture->focus = true;
            }
            else
            {
                IO::removeKeyCallback(callbackHandle);
                callbackHandle = IO::submitKeyCallback(cb);
                forCapture->focus = true;
            }
        }
    }
}
