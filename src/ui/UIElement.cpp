#include "ui/UIElement.h"

#include <string.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "graphics/rData.h"
#include "utils/Utils.h"

namespace MidiFi
{
    namespace UI
    {
        void UIElement::init(glm::vec3 pos, glm::vec4 color, float width, float height)
        {
            this->pos = pos;
            this->color = color;
            this->width = width;
            this->height = height;
        }
    }
}
