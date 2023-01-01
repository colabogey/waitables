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
