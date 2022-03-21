/* date = December 31st 2021 3:06 pm */

#pragma once

#include "UIElement.h"

namespace MidiFi
{
    namespace UI
    {
        class Icon : public UIElement
        {
            public:
            Icon() = default;
            Graphics::Texture tex = {nullptr};

            int toRData(Graphics::rData &r, unsigned int rOffset);
            void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);
            
            private:
            Graphics::Primitive prim = Graphics::Primitives::QUAD;
        };
    }
}