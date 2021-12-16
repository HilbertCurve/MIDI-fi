/* date = December 10th 2021 7:05 pm */

#pragma once

#include <GL/gl.h>

namespace MidiFi
{
    namespace Graphics
    {
        struct Font
        {
            const char *filepath;
            int fontSize;
            // this is for finding kern info, advance, and other font
            // metrics. used mainly by stb_truetype.h
            void *info;
            void *characters[96];

            GLuint texID;
            bool beingUsed;
        };
        struct Glyph
        {
            Font *parent;
            float *texCoords;
            float width, height;
        };

        void initFont(Font &f, const char *filepath, unsigned int fontSize);
        void bindFont(Font &f);
        void unbindFont(Font &f);
        Glyph getGlyph(Font &f, const char c);
    }
}