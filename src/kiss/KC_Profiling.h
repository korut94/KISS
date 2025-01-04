#pragma once

#if IS_DEBUG_BUILD
#include "KC_ScopedProfileTimer.h"

namespace KC_Profiling
{
    extern const char* ourProfileGameTag;
    extern const char* ourProfileGameRenderSyncTag;
    extern const char* ourProfileRenderTag;
    extern const char* ourProfileRenderDrawTag;
}

#define KC_PROFILE_BEGIN(aProfileTimerName)\
    KC_ProfileTimer __profileTimer{ aProfileTimerName };\
    __profileTimer.ResetTime();
#define KC_PROFILE_END()\
    __profileTimer.RecordTime();

#define KC_PROFILE(aProfileTimerName) KC_ScopedProfileTimer __scopedProfileTimer{ aProfileTimerName };

#define KC_PROFILE_GAME KC_PROFILE(KC_Profiling::ourProfileGameTag)
#define KC_PROFILE_GAMERENDERSYNC KC_PROFILE(KC_Profiling::ourProfileGameRenderSyncTag)
#define KC_PROFILE_RENDER KC_PROFILE(KC_Profiling::ourProfileRenderTag)
#define KC_PROFILE_RENDERDRAW KC_PROFILE(KC_Profiling::ourProfileRenderDrawTag)

#else
#define KC_PROFILE_BEGIN(aProfileTimerName)
#define KC_PROFILE_END()
#define KC_PROFILE(aProfileTimerName)
#define KC_PROFILE_GAME
#define KC_PROFILE_GAMERENDERSYNC
#define KC_PROFILE_RENDER
#define KC_PROFILE_RENDERDRAW
#endif // IS_DEBUG_BUILD
