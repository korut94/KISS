#include "KC_ProfileBlockTimes.h"

bool operator<(const KC_ProfileBlockTimes& a, const KC_ProfileBlockTimes& b)
{
    return a.myThreadId < b.myThreadId || a.myStartTime < b.myStartTime;
}

bool operator>(const KC_ProfileBlockTimes &a, const KC_ProfileBlockTimes &b)
{
    return a.myThreadId > b.myThreadId || a.myStartTime > b.myStartTime;
}
