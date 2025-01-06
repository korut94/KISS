#include "KC_ScopedProfileBlock.h"

KC_ScopedProfileBlock::KC_ScopedProfileBlock(const char* aName)
    : myBlock(aName)
{
    myBlock.ResetTime();
}

KC_ScopedProfileBlock::~KC_ScopedProfileBlock()
{
    myBlock.RecordTime();
}
