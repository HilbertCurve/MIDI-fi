#include "graphics/Font.h"

int main()
{
    MidiFi::Graphics::Font f;
    MidiFi::Graphics::initFont(f, "JetBrainsMono-Medium.ttf", 50);

    MidiFi::Graphics::Glyph g;
    MidiFi::Graphics::getGlyph(f, 'c');

    return 0;
}