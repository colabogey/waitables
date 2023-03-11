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

#define vpgnixEventIdStart 1000
#define corethreadEventIdStart 2000
#define aspthreadEventIdStart 3000
#define commonEventIdStart 9000

namespace vpgnixEventIds {
static const auto stopEventId = vpgnixEventIdStart;
static const auto runWaitableThreadId = vpgnixEventIdStart + 1;
static const auto messageSinkId = vpgnixEventIdStart + 2;
}; // namespace vpgnixEventIds

namespace corethreadEventIds {
static const auto stopEventId = corethreadEventIdStart;
static const auto runWaitableThreadId = corethreadEventIdStart + 1;
static const auto messageSinkId = corethreadEventIdStart + 2;
}; // namespace corethreadEventIds

namespace aspthreadEventIds {
static const auto stopEventId = aspthreadEventIdStart;
static const auto runWaitableThreadId = aspthreadEventIdStart + 1;
static const auto messageSinkId = aspthreadEventIdStart + 2;
}; // namespace aspthreadEventIds

namespace commonEventIds {
static const auto timeoutEventId = commonEventIdStart;
static const auto waitForAllEventId = commonEventIdStart + 1;
}; // namespace commonEventIds
