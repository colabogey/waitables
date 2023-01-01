#pragma once
#include <string>
#include <exception>

class vpgnixexception : public std::exception {
  public:
    vpgnixexception(const std::string&);
    virtual ~vpgnixexception() = default;
    vpgnixexception& operator=(vpgnixexception& rhs) = default;
    virtual const char* what() const throw();

private:
    std::string m_ex;
};
