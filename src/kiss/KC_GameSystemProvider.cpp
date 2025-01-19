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

float KC_GameSystemProvider::GetElapsedTime() const
{
    return myElapsedTime.AsSeconds();
}

const std::vector<KC_SpatialGrid>& KC_GameSystemProvider::GetSpatialGrids() const
{
    return myWorld.GetSpatialGrids();
}

std::vector<KC_SpatialGrid>& KC_GameSystemProvider::GetSpatialGrids()
{
    return myWorld.GetSpatialGrids();
}
