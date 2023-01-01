#pragma once
#include <stdlib.h>
#include <string>

class cstring {
  public:
    cstring();
    cstring(std::string);
    ~cstring();
    cstring(cstring& rhs);
    cstring& operator=(cstring rhs);
    cstring& operator+=(std::string&);
    cstring& operator+=(cstring&);
    cstring operator+(std::string&);
    cstring operator+(cstring&);
    void format(std::string sData, ...);
    //char* data() const;
    char* data() ;
    int size() const;

  private:
    char* m_cp;
};
