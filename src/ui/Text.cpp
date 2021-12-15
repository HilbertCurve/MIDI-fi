#include "ui/Text.h"

#include "graphics/Font.h"

#include "graphics/rData.h"
#include "utils/Utils.h"

namespace MidiFi
{
    namespace UI
    {
        void Text::init(glm::vec3 pos, glm::vec4 color, float width, float height, std::string text, Graphics::Font &f)
        {
            this->pos = pos;
            this->color = color;
            this->width = width;
            this->height = height;
            this->text = text;
            this->font = &f;
        }

        void Text::toRData(Graphics::rData &r, unsigned int rOffset)
        {
            using namespace MidiFi::Graphics;

            __pAssert(!(rOffset + this->text.size() * 4 >= r.vertCount / 4), "rData not big enough to hold UI data!");

            int stride = rOffset * getLayoutLen(r) * 4;

            // for each character in the text, add a default quad
            for (char c : this->text)
            {
                Glyph g = getGlyph(*this->font, c);
                /*
                
                for (int i = 0; i < 4; i++)
                {

                }
                */
                
                glm::vec3 posAccumulate = {0.0f, 0.0f, 0.0f};

                for (int i = 0; i < 4; i++)
                {
                    glm::vec3 orientation;
                    switch (i)
                    {
                        case 0: orientation = {1.0f * g.width, 1.0f * g.height, 0.0f}; break;
                        case 1: orientation = {0.0f * g.width, 1.0f * g.height, 0.0f}; break;
                        case 2: orientation = {0.0f * g.width, 0.0f * g.height, 0.0f}; break;
                        case 3: orientation = {1.0f * g.width, 0.0f * g.height, 0.0f}; break;
                    }

                    off_len result = getAttribMetaData(r, PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        this->pos += orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f} + posAccumulate;

                        // TODO: just use memcpy, bonehead.
                        for (int j = 0; j < 3; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&this->pos)[j];
                        }

                        this->pos -= orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f} - posAccumulate;
                    }
                    
                    result = getAttribMetaData(r, PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&this->color)[j];
                        }             
                    }

                    result = getAttribMetaData(r, PONT_TEXCOORD);
                    
                    if (result.second >= 2 * sizeof(float))
                    {
                        orientation.x /= this->width;
                        orientation.y /= this->height;
                        for (int j = 0; j < 2; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = g.texCoords[i * 4 + j];
                        }
                    }

                    result = getAttribMetaData(r, PONT_TEXID);
                    if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                    {
                        if (this->tex.source == nullptr)
                        {
                            *(float *)((char *)r.data + result.first + stride) = 0.0f;
                        }
                        else
                        {
                            *(float *)((char *)r.data + result.first + stride) = g.parent->texID;
                        }
                    }
                    stride += getLayoutLen(r);

                    posAccumulate += g.width;

                    // if align is to the left
                    if (posAccumulate.x > this->width)
                    {
                        posAccumulate.x = 0;
                        posAccumulate.y += g.parent->fontSize;
                    }
                }
            }
        }
    }
}