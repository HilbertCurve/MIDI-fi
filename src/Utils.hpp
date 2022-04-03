/* date = March 21 2022 2:28 pm */

#ifndef MIDIFI_UTILS_HPP
#define MIDIFI_UTILS_HPP

#include <cstdio>
#include <cstdlib>

#define m_log_message(format, ...)                          \
{                                                           \
printf("\x1B[01m\x1B[32m[INFO]: Log at file %s, line %d:\x1B[0m ", __FILE__, __LINE__);\
printf(format __VA_OPT__(,) __VA_ARGS__);                   \
printf("\n");                                               \
}

#define m_log_warning(format, ...) \
{\
fprintf(stderr, "\x1B[01m\x1B[31m[WARNING]: Warning at file %s, line %d:\x1B[0m ", __FILE__, __LINE__);\
fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__);\
fprintf(stderr, "\n");\
}

#define m_assert(condition, fail, ...)                      \
{                                                           \
if (!(condition)) {                                         \
fprintf(stderr, "\x1B[01m\x1B[31m[FATAL]: Assertion failed at file %s, line %d:\x1B[0m ", __FILE__, __LINE__);\
fprintf(stderr, fail __VA_OPT__(,) __VA_ARGS__);            \
fprintf(stderr, "\n");                                      \
exit(-1);                                                   \
}                                                           \
}

#define m_log_error(format, ...)                            \
{                                                           \
fprintf(stderr, "\x1B[01m\x1B[31m[FATAL]: Error occured at file %s, line %d:\x1B[0m ", __FILE__, __LINE__);\
fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__);          \
fprintf(stderr, "\n");                                      \
exit(-1);                                                   \
}

namespace MidiFi {
    class String {
        public:
        String();
        String(const char *src);
        ~String();
        String &operator=(const String &s);
        String &operator=(const char *s);
        char *src;
        int len;
    };
}

#endif // MIDIFI_UTILS_HPP

