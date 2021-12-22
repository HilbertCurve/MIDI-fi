#include "graphics/Font.h"

#include <string>
#include <glm/glm.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#include "core/Application.h"
#include "utils/Utils.h"

namespace MidiFi
{
    namespace Graphics
    {
        void initFont(Font &f, const char *fontname, unsigned int fontSize)
        {
            f.texID = fontPoolStackPointer + 8;

            /* Load font (. ttf) file */

            File fontFile;
            loadFile(fontname, fontFile, true);

            stbtt_fontinfo info;
            if (!stbtt_InitFont(&info, (unsigned char *) fontFile.buffer, 0))
            {
                printf("stb init font failed\n");
            }

            f.info = info;

            int bitmap_w = 512;
            int bitmap_h = 512;
            unsigned char *bitmap = (unsigned char *)malloc(bitmap_w * bitmap_h);

            /* Calculate font scaling */
            f.fontSize = fontSize;
            float pixels = fontSize;
            float scale = stbtt_ScaleForPixelHeight(&info, pixels); /* scale = pixels / (ascent - descent) */

            /*
             * Get the measurement in the vertical direction
             * ascent: The height of the font from the baseline to the top;
             * descent: The height from baseline to bottom is usually negative;
             * lineGap: The distance between two fonts;
             * The line spacing is: ascent - descent + lineGap.
             */
            stbtt_GetFontVMetrics(&info, &f.ascent, &f.descent, &f.lineGap);

            /* Adjust word height according to zoom */
            f.ascent = roundf(f.ascent * scale);
            f.descent = roundf(f.descent * scale);

            /* Cyclic loading of each character in word */
            //for (int i = 33; i < 126; ++i)
            //{
            // this works, but I'm gonna use baked fonts for now

            //     /*
            //      * Get the measurement in the horizontal direction
            //      * advanceWidth: currnet codepoint position;
            //      * leftSideBearing: Left side position;
            //      */
            //     stbtt_GetCodepointHMetrics(&info, (char)i, &advanceWidth, &leftSideBearing);

            //     // ensure we don't go out of bounds
            //     if (x + advanceWidth * scale > 512)
            //     {
            //         y += pixels;
            //         x = 0;
            //     }

            //     /* Gets the border of a character */
            //     int c_x1, c_y1, c_x2, c_y2;
            //     stbtt_GetCodepointBitmapBox(&info, (char)i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

            //     /* Calculate the y of the bitmap (different characters have different heights) */
            //     int h = ascent + c_y1 + y;

            //     /* Render character */
            //     int byteOffset = x + roundf(leftSideBearing * scale) + (h * bitmap_w);
            //     stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_w, scale, scale, (char) i);

            //     /* Adjust x */
            //     x += roundf(advanceWidth * scale);

            //}
            
            stbtt_BakeFontBitmap((unsigned char *) fontFile.buffer, 0, 32.0, bitmap,
                                 512, 512, 32, 96,
                                 f.characters);

            ///* Save the bitmap data to the 1-channel png image */
            // std::string fontpath = "assets/fonts/bin/" + std::string(fontname).substr(0, std::string(fontname).length() - 4) + ".png";
            // stbi_write_png("assets/fonts/bin/test.png", 512, 512, 1, bitmap, 512);

            // at this point we can use bitmap as a gl texture
            unsigned int id = f.texID;
            glGenTextures(1, &(f.texID));
            glBindTexture(GL_TEXTURE_2D, f.texID);

            // GL_RED here refers to the fact that there's only one channel (the first one)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

            // a swizzle maps some channels of a texture to different ones (in this case, we turn our
            // texture from a red-intensity to an rgba-intensity.) this lets the shader/vertex data
            // choose the color of the text.
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            fontPool[fontPoolStackPointer] = &f;
            fontPoolStackPointer++;

            freeFile(fontFile);
            free(bitmap);
        }

        void bindFont(Font &f)
        {
            glBindTexture(GL_TEXTURE_2D, f.texID);
            f.beingUsed = true;
        }

        void unbindFont(Font &f)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            f.beingUsed = false;
        }

        Glyph getGlyph(Font &f, const char c)
        {
            Glyph ret;

            ret.parent = &f;

            stbtt_aligned_quad q;
            // i don't really NEED these too much; all positioning is handled by the camera
            float dummyx = 0, dummyy = 0;
            stbtt_GetBakedQuad(f.characters, 512, 512, c - 32, &dummyx, &dummyy, &q, true);

            float texcoords[] =
                {
                    q.s1, q.t1,
                    q.s0, q.t1,
                    q.s0, q.t0,
                    q.s1, q.t0
                };
            
            memcpy(&ret.texCoords, texcoords, 8 * sizeof(float));

            glm::vec2 widthAndHeight = c != ' ' ? 
                screenToWorldSize(glm::vec2{q.x1 - q.x0, q.y1 - q.y0}) : 
                screenToWorldSize(glm::vec2{f.fontSize, 0.0f});

            ret.width = widthAndHeight.x;
            ret.height = widthAndHeight.y;

            return ret;
        }
    }
}