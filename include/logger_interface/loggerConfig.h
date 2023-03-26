/* Copyright 2022-Present Steve Yackey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once
#include <stdint.h>
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
