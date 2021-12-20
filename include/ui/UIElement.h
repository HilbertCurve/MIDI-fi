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
            UIElement() = default;

            glm::vec3 pos;
            glm::vec4 color;
            float width, height;

            Graphics::Texture tex = {nullptr};
            Graphics::Primitive prim = Graphics::Primitives::QUAD;
            
            // should I overload contructor?? it's basically the same as {} contruction
            void init(glm::vec3 pos, glm::vec4 color, float width, float height);
            int toRData(Graphics::rData &r, unsigned int rOffset);
            void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);
        };
    }
}
