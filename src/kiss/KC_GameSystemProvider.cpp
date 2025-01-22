#include "KC_GameSystemProvider.h"

#include "KC_World.h"

namespace KC_GameSystemProvider_Private
{
    static constexpr const float locFixedUpdateTimeSec = 1.f / 60.f;
}

KC_Time KC_GameSystemProvider::ourFixedUpdateTime = KC_Time::Seconds(KC_GameSystemProvider_Private::locFixedUpdateTimeSec);

KC_GameSystemProvider::KC_GameSystemProvider(KC_World& aWorld)
    : Super(aWorld.GetComponentManager())
    , myWorld(aWorld)
{
}
