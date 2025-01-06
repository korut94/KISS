#pragma once

#include "KC_ProfileBlock.h"

class KC_ScopedProfileBlock final
{
public:
    KC_ScopedProfileBlock(const char* aName);
    ~KC_ScopedProfileBlock();

private:
    KC_ProfileBlock myBlock;
};
