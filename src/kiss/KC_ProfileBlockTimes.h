#pragma once

#include "KC_Time.h"

#include <thread>

struct KC_ProfileBlockTimes
{
    std::thread::id myThreadId;
    KC_Time myStartTime;
    KC_Time myEndTime;
};
