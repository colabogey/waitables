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
