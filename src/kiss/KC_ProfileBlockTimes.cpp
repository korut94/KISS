#include "KC_ProfileBlockTimes.h"

bool operator<(const KC_ProfileBlockTimes& a, const KC_ProfileBlockTimes& b)
{
    if (a.myThreadId < b.myThreadId)
        return true;
    if (a.myThreadId == b.myThreadId && a.myStartTime < b.myStartTime)
        return true;
    else
        return false;
}

bool operator>(const KC_ProfileBlockTimes& a, const KC_ProfileBlockTimes& b)
{
    if (a.myThreadId > b.myThreadId)
        return true;
    if (a.myThreadId == b.myThreadId && a.myStartTime > b.myStartTime)
        return true;
    else
        return false;
}
