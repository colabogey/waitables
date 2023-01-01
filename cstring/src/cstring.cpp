#include "cstring.h"
#include <string.h>
#include <stdarg.h>

cstring::cstring() { 
    m_cp = (char*)calloc(sizeof(char), 1);
}

cstring::cstring(std::string s) { 
    m_cp = (char*)calloc(sizeof(char), s.size());
    strcat(m_cp, s.c_str());
}

cstring::cstring(cstring& rhs) {
    m_cp = (char*)calloc(sizeof(char), rhs.size());
    strcpy(m_cp, rhs.data());
}

cstring::~cstring() { 
    if(m_cp != nullptr)  {
        free(m_cp);
        m_cp = nullptr;
    }
}

cstring& cstring::operator=(cstring rhs) {
    m_cp = (char*)realloc(m_cp, rhs.size());
    strcpy(m_cp, rhs.data());
    return *this;
}

cstring& cstring::operator+=(std::string& s) {
    m_cp = (char*)realloc(m_cp, s.size());
    strcat(m_cp, s.c_str());
    return *this;
}

cstring& cstring::operator+=(cstring& s) {
    m_cp = (char*)realloc(m_cp, s.size());
    strcat(m_cp, s.data());
    return *this;
}

cstring cstring::operator+(std::string& s) {
    cstring c;
    c.m_cp = (char*)realloc(m_cp, s.size());
    strcat(c.data(), s.data());
    return c;
}

cstring cstring::operator+(cstring& s) {
    cstring c;
    c.m_cp = (char*)realloc(m_cp, s.size());
    strcat(c.data(), s.data());
    return c;
}

void cstring::format(std::string sData, ...) {
    va_list args1;
    va_start(args1, sData);
    va_list args2;
    va_copy(args2, args1);
    int sz = (1 + std::vsnprintf(nullptr, 0, sData.c_str(), args1));
    m_cp = (char*)realloc(m_cp, sz);
    va_end(args1);
    vsnprintf(m_cp, sz, sData.c_str(), args2);
    va_end(args2);
}

//char* cstring::data() const {
char* cstring::data() {
    return m_cp;
}

int cstring::size() const {
    return strlen(m_cp);
}

