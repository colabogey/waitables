#include <vpgnixexception.h>

vpgnixexception::vpgnixexception(const std::string& ex) {
    m_ex = ex;
}

const char* vpgnixexception::what() const throw(){
    return m_ex.c_str();
}

