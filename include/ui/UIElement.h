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
        class UIElement
        {
            public:
            glm::vec3 pos;
            glm::vec4 color;
            float width, height;
            
            // should I overload contructor?? it's basically the same as {} contruction
            void init(glm::vec3 pos, glm::vec4 color, float width, float height);
            virtual int toRData(Graphics::rData &r, unsigned int rOffset) = 0;
            virtual void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property) = 0;
        };
    }
}
