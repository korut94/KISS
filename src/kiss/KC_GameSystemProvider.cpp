#include "KC_GameSystemProvider.h"

namespace KC_GameSystemProvider_Private
{
    static constexpr const float locFixedUpdateTimeSec = 1.f / 60.f;
}

KC_Time KC_GameSystemProvider::ourFixedUpdateTime = KC_Time::Seconds(KC_GameSystemProvider_Private::locFixedUpdateTimeSec);

KC_GameSystemProvider::KC_GameSystemProvider(KC_MainComponentManager& aComponentManager)
    : Super(aComponentManager)
{
}

float KC_GameSystemProvider::GetElapsedTime() const
{
    return myElapsedTime.AsSeconds();
}
