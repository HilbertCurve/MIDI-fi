#include "ui/Text.h"

#include "graphics/Font.h"

#include "graphics/rData.h"
#include "utils/Utils.h"

namespace MidiFi
{
    namespace UI
    {
        void Text::init(glm::vec3 pos, glm::vec4 color, float width, float height, std::string text, Graphics::Font &f)
        {
            this->pos = pos;
            this->color = color;
            this->width = width;
            this->height = height;
            this->text = text;
            this->font = &f;
        }

        void Text::toRData(Graphics::rData &r, unsigned int rOffset)
        {
            using namespace MidiFi::Graphics;

            __pAssert(!(rOffset + this->text.size() * 4 >= r.vertCount / 4), "rData not big enough to hold game states!");

            int stride = rOffset * getLayoutLen(r) * 4;

            // for each character in the text, add a default quad
            for (char c : this->text)
            {
                Glyph g = getGlyph(*this->font, c);
                
            }
        }
    }
}
