/* date = October 30th 2021 3:16 pm */

#pragma once

#include <vector>

#include "ui/UIElement.h"

namespace MidiFi
{
    namespace UI
    {
        typedef void (* _init)();
        typedef void (* _update)(double dt);

        struct Scene
        {
            _init init;
            _update update; 

            std::vector<UIElement *> objs = std::vector<UIElement *>();
        };

        void init();
        void update(double dt);

        Scene *getScene();
    }
}