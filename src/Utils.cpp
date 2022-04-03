#include "Utils.hpp"

#include <cstring>
#include "Memory.hpp"

namespace MidiFi {
    String::String() {
        this->len = 0;
        this->src = nullptr;
    }
    String::String(const char *s) {
        this->len = strlen(s);
        this->src = (char *) m_malloc(len + 1, "string allocation");
        memcpy(this->src, s, this->len + 1);
    }
    String::~String() {
        m_free((void *) this->src);
    }
    String &String::operator=(const String &s) {
        if (this->src) m_free((void *) this->src);
        this->len = s.len;
        this->src = (char *) m_malloc(this->len + 1, "string allocation");
        memcpy(this->src, s.src, this->len + 1);
        return *this;
    }
    /*
    String String::operator=(String s) {
        if (this->src) m_free((void *) this->src);
        this->src = s.src;
        this->len = s.len;
        return *this;
    }
    */
    String &String::operator=(const char *s) {
        if (this->src) m_free((void *) this->src);
        this->len = strlen(s);
        this->src = (char *) m_malloc(len + 1, "string allocation");
        memcpy(this->src, s, this->len + 1);
        return *this;
    }
}

