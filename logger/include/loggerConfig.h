#pragma once
#include <string>
#include <vector>

class loggerConfig {
public:
    loggerConfig();
    virtual ~loggerConfig() = default;
    void init(std::string, std::string, std::string, uint32_t, uint32_t, uint32_t, uint32_t);
    std::string getFn() { return m_fn; };
    uint32_t getLvl() { return m_lvl; };
private:
    void setFnFromBase();
    std::string m_dn;
    std::string m_fnBase;
    std::string m_fn;
    uint32_t m_genSuffix;
    uint32_t m_lvl;
    uint32_t m_size;
    uint32_t m_gens;
};
