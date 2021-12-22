/* date = December 10th 2021 7:05 pm */

#pragma once

#include <GL/gl.h>

#include <stb/stb_truetype.h>

namespace MidiFi
{
    namespace Graphics
    {
        struct Font
        {
            const char *filepath;
            union
            {
                int width;
                int fontSize;
            };
            // is this bloat? i know i can just use stbtt_GetFontVMetrics, but this could be faster...
            int ascent;
            int descent;
            int lineGap;

            // this is for finding kern info, advance, and other font
            // metrics. used mainly by stb_truetype.h
            stbtt_fontinfo info;
            stbtt_bakedchar characters[96];

            GLuint texID;
            bool beingUsed;
        };
        struct Glyph
        {
            Font *parent;
            struct 
            {
                float a0, a1;
                float a2, a3;
                float a4, a5;
                float a6, a7;
            } texCoords;
            float width, height;
        };

        void initFont(Font &f, const char *filepath, unsigned int fontSize);
        void bindFont(Font &f);
        void unbindFont(Font &f);
        Glyph getGlyph(Font &f, const char c);
    }
}