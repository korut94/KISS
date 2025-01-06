#pragma once

#include "KC_ProfileBlockTimes.h"

class KC_ProfileBlock final
{
public:
    explicit KC_ProfileBlock(const char* aName);

    void ResetTime();
    void RecordTime();

private:
    const char* const myName;
    KC_ProfileBlockTimes myTimes;
};
