#include <Utils.hpp>

#include <cstring>

int main() {
    using namespace MidiFi;
    String s;
    String a = "Test";

    m_assert(a.len == 4, "Function `String::operator=(const char *)` failed 1st assertion.");
    m_assert(strcmp("Test", a.src) == 0, 
            "Function `String::operator=(const char *) failed 2nd assertion.`");
    s = a;

    m_assert(s.len == 4, "Function `String::operator=(String &)` failed 1st assertion.");
    m_assert(strcmp("Test", s.src) == 0, 
            "Function `String::operator=(const String &) failed 2nd assertion.`");
    s = "TestAgain";

    m_assert(s.len == 9, "Function `String::operator=(const char *)` failed 3rd assertion.");
    m_assert(strcmp("TestAgain", s.src) == 0, 
            "Function `String::operator=(const char *) failed 2nd assertion.`");

}

