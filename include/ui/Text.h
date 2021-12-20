/* date = December 13th 2021 9:01 pm */

#pragma once

#include <string>

#include "graphics/Font.h"
#include "ui/UIElement.h"

namespace MidiFi
{
    namespace UI
    {
        class Text : public UIElement
        {
            public:
            std::string text;
            Graphics::Font *font;
            void init(glm::vec3 pos, glm::vec4 color, float width, float height, std::string text, Graphics::Font &f);
            int toRData(Graphics::rData &r, unsigned int rOffset);
        };
    }
}