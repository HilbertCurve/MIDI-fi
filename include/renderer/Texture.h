/* date = October 11th 2021 2:35 pm */

#ifndef _PONTILUS_TEXTURE_H
#define _PONTILUS_TEXTURE_H

#include <GL/gl.h>

namespace Pontilus
{
    namespace Graphics
    {
        struct IconMap
        {
            const char *filepath;
            GLuint texID;
            GLint width, height;
            GLint textureWidth;
            bool beingUsed;
        };

        struct Texture
        {
            IconMap source;
            float texCoords[4];
        };

        // assumes element width and element height are the same.
        void initIconMap(const char *filepath, IconMap &tex, int textureWidth);

        void bindIconMap(Texture &t);
        void unbindIconMap(Texture &t);

        Texture getTexture(IconMap &im, int index);
    }
}

#endif