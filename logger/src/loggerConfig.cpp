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

#include <vector>
#include <loggingLevel.h>
#include <loggerConfig.h>

loggerConfig::loggerConfig() {
    m_dn = "/tmp";
    m_fnBase = "vpgnixDaemon";
    m_genSuffix = 0;
    m_lvl = LOGLEVEL_INFO;
    m_size = 1024 * 1024 * 5;
    m_gens = 3;
    setFnFromBase();
}

//
// TODO: init should be called by whatever reads a configuration file
//
void loggerConfig::init(std::string fn, std::string dn, std::string fnBase, uint32_t genSuffix, uint32_t lvl, uint32_t size, uint32_t gens)
{
    m_dn = dn;
    m_fnBase = fnBase;
    m_fn = fn;
    m_genSuffix = genSuffix;
    m_lvl = lvl;
    m_size = size;
    m_gens = gens;
    setFnFromBase();
}

void loggerConfig::setFnFromBase() {
    int size = snprintf(nullptr, 0, "%s/%s_%d.log", 
                        m_dn.c_str(), m_fnBase.c_str(), m_genSuffix); 
    std::vector<char> ln(size+1);
    snprintf(ln.data(), ln.size(), "%s/%s_%d.log", 
             m_dn.c_str(), m_fnBase.c_str(), m_genSuffix); 
    m_fn = ln.data();
}
