#include "KC_GameSystemProvider.h"

KC_GameSystemProvider::KC_GameSystemProvider(KC_MainComponentManager& aComponentManager)
    : Super(aComponentManager)
{
}

float KC_GameSystemProvider::GetElapsedTime() const
{
    return myElapsedTime.AsSeconds();
}
