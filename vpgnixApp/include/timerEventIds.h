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

#define waitableTimerEventIdStart 1000
#define corethreadWaitableTimerEventIdStart waitableTimerEventIdStart + 0
#define aspthreadWaitableTimerEventIdStart waitableTimerEventIdStart + 100
#define postableTimerEventIdStart waitableTimerEventIdStart + 1000
#define aspthreadPostableTimerEventIdStart postableTimerEventIdStart

namespace corethreadWaitableTimerEventIds {
// static const auto corethreadWaitableTimer =
// corethreadWaitableTimerEventIdStart;
// static const auto another = corethreadWaitableTimerEventIdStart + 1;
// etc
}; // namespace corethreadWaitableTimerEventIds

namespace aspthreadPostableTimerEventIds {
static const auto ASPThreadHealth = aspthreadPostableTimerEventIdStart;
// static const auto another = aspPostableTimerEventIdStart + 1;
// etc
}; // namespace aspthreadPostableTimerEventIds

namespace aspthreadWaitableTimerEventIds {
static const auto aspthreadWaitableTimer = aspthreadWaitableTimerEventIdStart;
// static const auto another = aspWaitableTimerEventIdStart + 1;
// etc
}; // namespace aspthreadWaitableTimerEventIds
