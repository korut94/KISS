#pragma once

#include "KC_Time.h"

#include <thread>

struct KC_ProfileBlockTimes
{
    std::thread::id myThreadId;
    KC_Time myStartTime;
    KC_Time myEndTime;
};

bool operator<(const KC_ProfileBlockTimes& a, const KC_ProfileBlockTimes& b);
bool operator>(const KC_ProfileBlockTimes& a, const KC_ProfileBlockTimes& b);
