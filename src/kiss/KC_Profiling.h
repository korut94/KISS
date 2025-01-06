#pragma once

#if IS_DEBUG_BUILD
#include "KC_ScopedProfileBlock.h"

#define KC_PROFILE_BEGIN(aProfileBlockName)\
    KC_ProfileBlock __profileBlock{ aProfileBlockName };\
    __profileBlock.ResetTime();
#define KC_PROFILE_END()\
    __profileBlock.RecordTime();

#define KC_PROFILE(aProfileBlockName) KC_ScopedProfileBlock __scopedProfileBlock{ aProfileBlockName };

#define KC_PROFILE_STEP "::"

#define KC_PROFILE_GAME KC_PROFILE("Game")
#define KC_PROFILE_GAMERENDERSERVICEPROVIDER KC_PROFILE("Game" KC_PROFILE_STEP "RenderServiceProdiver")
#define KC_PROFILE_GAMEUPDATE KC_PROFILE("Game" KC_PROFILE_STEP "Update")
#define KC_PROFILE_RENDER KC_PROFILE("Render")
#define KC_PROFILE_RENDERDRAW KC_PROFILE("Render" KC_PROFILE_STEP "Draw")

#else
#define KC_PROFILE_BEGIN(aProfileBlockName)
#define KC_PROFILE_END()
#define KC_PROFILE(aProfileBlockName)
#define KC_PROFILE_STEP
#define KC_PROFILE_GAME
#define KC_PROFILE_GAMESERVICEPROVIDER
#define KC_PROFILE_RENDER
#define KC_PROFILE_RENDERDRAW
#endif // IS_DEBUG_BUILD
