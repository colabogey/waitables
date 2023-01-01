#pragma once
#include <unistd.h>

typedef struct type_waitable_message {
    uint32_t	message;
    uint64_t	wParam;
    uint64_t	lParam;
} waitable_message;
