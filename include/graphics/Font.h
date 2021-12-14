/* date = December 10th 2021 7:05 pm */

#include <GL/gl.h>
#include <stb/stb_truetype.h>

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
            stbtt_fontinfo info;
            stbtt_bakedchar characters[96];
            GLuint texID;
        };
        struct Glyph
        {
            Font *parent;
            float *texCoords;
        };

        void initFont(Font &f, const char *filepath, unsigned int fontSize);
        Glyph getGlyph(Font &f, const char c);
    }
}