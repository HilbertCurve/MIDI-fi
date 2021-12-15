#include "utils/Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>

#include "graphics/Camera.h"
#include "core/Application.h"

namespace MidiFi
{
    void loadFile(const char *filepath, File &fb, bool isBinary)
    {
        fb.isBinary = isBinary;

        FILE *f;

        if (isBinary)
        {
            f = fopen(filepath, "rb");
        }
        else
        {
            f = fopen(filepath, "r");
        }

        fseek(f, 0, SEEK_END);
        fb.size = ftell(f);
        fseek(f, 0, SEEK_SET);

        fb.buffer = malloc(fb.size);

        fread(fb.buffer, fb.size, (size_t)1, f);

        fclose(f);
    }

    using namespace glm;
    vec3 screenToWorldCoords(vec2 screenPos)
    {
        vec4 v = {screenPos, 0.0f, 1.0f};

        v -= vec4{window.width / 2, window.height / 2, 0.0f, 0.0f};
        v /= vec4{window.width / 2, -window.height / 2, 1.0f, 1.0f};
        
        v = inverse(Renderer::Camera::getView()) * inverse(Renderer::Camera::getProjection()) * v;

        return {v.x, v.y, v.z};
    }
}