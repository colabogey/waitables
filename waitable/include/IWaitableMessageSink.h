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
#include <IMessageSink.h>
#include <ISourceSinkIdentity.h>
#include <IMessageSinkSetup.h>
#include <waitable_message.h>

class IWaitableMessageSink : public IMessageSink, public IMessageSinkSetup, 
                             public ISourceSinkIdentity, public IWaitable {
public:
    virtual bool peekMessage(waitable_message*, bool) = 0;
    virtual bool getMessage(waitable_message*) = 0;
    virtual ~IWaitableMessageSink() = default;
};
