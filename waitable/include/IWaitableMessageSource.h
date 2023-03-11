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
#include <IMessageSource.h>
#include <ISourceSinkIdentity.h>
#include <IMessageSourceSetup.h>

class IWaitableMessageSource : public IMessageSource, public IMessageSourceSetup, 
                               public ISourceSinkIdentity, public IWaitable {
public:
    virtual bool postThreadMessage(std::string, uint32_t, uint64_t, uint64_t) = 0;
    virtual bool postMessage(uint32_t, uint64_t, uint64_t) = 0;
    virtual ~IWaitableMessageSource() = default;
};
