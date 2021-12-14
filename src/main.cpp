#include <string.h>

#include "core/Application.h"

int main(int argc, char *args[])
{
    if (argc > 0)
    {
        MidiFi::_MIDIFI_SETTINGS *a = MidiFi::getArgs();
        for (int i = 0; i < argc; i++)
        {
            if (strcmp(args[i], "debug") == 0)
            {
                *a |= 0x0001;
            }
        }
    }

    MidiFi::init();
    MidiFi::loop();

    return 0;
}
