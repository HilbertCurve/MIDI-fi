#include "ui/Scene.h"

#include "core/Application.h"
#include "ui/Text.h"

namespace MidiFi
{
    namespace UI
    {
        // I have no idea how to reap the benifits of oop with c++
        //void Scene::toRData(Graphics::rData &r, unsigned int rOffset)
        //{
        //    unsigned int currentOffset = rOffset;
        //    for (auto &obj : objs) // doesn't cast to specific type, does UIElement::toRData on a Text element
        //    {
        //        currentOffset += obj.toRData(r, currentOffset);
        //    }
        //}

        namespace Scenes
        {
            Scene debug = 
            {
                []()
                {
                    static UIElement g1 = UIElement();
                    static UIElement g2 = UIElement();
                    static UIElement g3 = UIElement();
                    static UIElement g4 = UIElement();
                    static Text t1;
                    static Graphics::IconMap im1;
                    static Graphics::IconMap im2;
                    static Graphics::IconMap blueberry;
                    static Graphics::IconMap burger;
                    static Graphics::IconMap pizzaMonster;
                    static Graphics::Font jetBrainsMono;
                    static Scene s = {};
                    g1.init({-7.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                    g2.init({-2.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                    g3.init({ 2.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                    g4.init({ 7.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                    Graphics::initIconMap("./assets/images/test.png", im1, 8, 8, 0);
                    Graphics::initIconMap("./assets/images/test2.png", im2, 8, 8, 0);
                    Graphics::initIconMap("./assets/images/blueberry.png", blueberry, 32, 32, 0);
                    Graphics::initIconMap("./assets/images/burger.png", burger, 32, 32, 0);
                    Graphics::initIconMap("./assets/images/pizzaMonster.png", pizzaMonster, 32, 32, 0);

                    Graphics::initFont(jetBrainsMono, "./assets/fonts/JetBrainsMono-Medium.ttf", 12);

                    g1.tex = Graphics::getTexture(blueberry, 0);
                    g3.tex = Graphics::getTexture(burger, 0);
                    g4.tex = Graphics::getTexture(pizzaMonster, 0);

                    t1.init({-10, -10, 0}, {1.0, 0.0, 1.0, 1.0}, 30, 30, "Hello World!", jetBrainsMono);

                    // the thing about stacks is that they copy the values being pushed, so I shouldn't get segfaults here
                    // for having local variables?
                    debug.objs.push_back(g1);
                    debug.objs.push_back(g2);
                    debug.objs.push_back(g3);
                    debug.objs.push_back(g4);
                    debug.objs.push_back(t1);
                    
                    int offset = 0;
                    debug.numQuads += g1.toRData(quadPool, debug.numQuads);
                    debug.numQuads += t1.toRData(quadPool, debug.numQuads);
                },
                [](double dt)
                {

                },
                []()
                {

                }
            };
        }
    }
}