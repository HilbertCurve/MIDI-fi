#include "graphics/Texture.h"

#include <math.h>
#include <glm/glm.hpp>

#include "core/Application.h"
#include "stb/stb_image.h"

namespace Pontilus
{
    namespace Graphics
    {

        void initIconMap(const char *filepath, IconMap &tex, int textureWidth, int textureHeight)
        {
            static int id = 0;
            tex.filepath = filepath;

            glGenTextures(1, &(tex.texID));
            glBindTexture(GL_TEXTURE_2D, tex.texID);

            // Set texture parameters
            // Repeat image in both directions
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // When stretching the image, pixelate
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            // When shrinking an image, pixelate
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            GLint *width = new int;
            GLint *height = new int;
            GLint *channels = new int{0};
            stbi_set_flip_vertically_on_load(true);

            stbi_uc *image = stbi_load(filepath, width, height, channels, 4);

            if (image != nullptr)
            {
                tex.width = *width;
                tex.height = *height;

                if (*channels == 3)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                else if (*channels == 4)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                else
                {
                    fprintf(stderr, "Unknown number of channels: \"%d\"\n", *channels);
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr, "Error: Could not load image \"%s\"\n", filepath);
                exit(-1);
            }

            stbi_image_free(image);
            tex.width = *width;
            tex.height = *height;

            if (debugMode())
            {
                printf("Rendering Image: %s\nWidth: %d\nHeight: %d\nNumber of Channels: %d\n", filepath, *width, *height, *channels);
                printf("texID: %d\n\n", tex.texID);
            }

            iconPool[id] = &tex;
            id++;

            tex.textureWidth = textureWidth;
            tex.textureHeight = textureHeight;

            delete width;
            delete height;
            delete channels;
        }

        void bindIconMap(IconMap &t)
        {
            glBindTexture(GL_TEXTURE_2D, t.texID);
            t.beingUsed = true;
        }

        void unbindIconMap(IconMap &t)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            t.beingUsed = false;
        }

        Texture getTexture(IconMap &im, int index)
        {
            Texture tex = {};
            tex.source = &im;
            // get offset from top
            int pixelsFromTop = std::floor(index * im.textureWidth / im.width) * im.textureHeight;
            // get offset from left
            int pixelsFromLeft = (index * im.textureWidth) % im.width;
            
            glm::vec2 pos1 = {pixelsFromLeft, pixelsFromTop};
            glm::vec2 pos2 = {pixelsFromLeft + im.textureWidth, pixelsFromTop + im.textureHeight};
            glm::vec2 center = {im.width / 2, im.height / 2};

            // get relative position based on center of iconmap
            pos1 -= center;
            pos1 /= center;
            
            // determine coords of bottom right point
            pos2 -= center;
            pos2 /= center;

            // insert texcoords for easy use in RData structs and OpenGL
            float coords[] = 
            {
                pos2.x, pos1.y,
                pos1.x, pos1.y,
                pos1.x, pos2.y,
                pos2.x, pos2.y
            };

            for (int i = 0; i < 8; i++) 
            {
                tex.texCoords[i] = coords[i];
            }

            return tex;
        }
    }
}