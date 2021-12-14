#include "utils/Utils.h"

#include <stdio.h>
#include <stdlib.h>

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
}