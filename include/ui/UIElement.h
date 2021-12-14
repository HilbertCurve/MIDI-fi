/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "graphics/rData.h"
#include "graphics/Primitive.h"
#include "graphics/Texture.h"

namespace MidiFi
{
    namespace UI
    {
        struct UIElement
        {
            glm::vec3 pos;
            glm::vec4 color;
            float width, height;

            Graphics::Texture tex = {nullptr};
            Graphics::Primitive prim = Graphics::Primitives::QUAD;
        };

        void initUIElement(UIElement &g, glm::vec3 pos, glm::vec4 color, float width, float height);

        void uiStateToRData(UIElement &s, Graphics::rData &r, unsigned int rOffset);
        void uiStateToRData(std::vector<UIElement> gs, Graphics::rData &r, unsigned int rOffset);
        void uiStateToRData(UIElement &s, Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);
    }
}