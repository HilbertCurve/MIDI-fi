/* date = December 13th 2021 9:01 pm */

#pragma once

#include <string>
#include <GLFW/glfw3.h>

#include "graphics/Font.h"
#include "graphics/rData.h"
#include "ui/UIElement.h"

namespace MidiFi
{
    namespace UI
    {
        // i'm putting this in Text.h instead of InputListener.h because we're INTERPRETING input data, rather than GETTING it.
        struct ShiftedKey { int GLFWKey; char shiftedChar; };

        /**
         * If there's a different encoding you'd like to use, make a new one 
         * below the above array and (preferably) send a pull request with it in there. I
         * only have one keyboard, which is USStandardEncodingShiftedKeys.
         * // TODO: gather lots of encodings and auto-detect them at program start or compile time.
         */
        extern ShiftedKey *currentEncoding;

        char getShiftedKey(int GLFWKey);
        bool isFunctionKey(int key);
        void setCurrentEncoding(ShiftedKey *encoding);

        class Text : public UIElement
        {
            public:
            std::string text;
            Graphics::Font *font;

            void init(glm::vec3 pos, glm::vec4 color, float width, float height, std::string text, Graphics::Font &f);

            int toRData(Graphics::rData &r, unsigned int rOffset);
            void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property) {};

            bool hasFocus();
            static void setFocus(Text &t); // TODO: move this to some kind of state machine

            private:
            bool focus = false;
        };
    }
}