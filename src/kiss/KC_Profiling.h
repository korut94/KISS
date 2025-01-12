#pragma once

#if IS_DEBUG_BUILD
#include "KC_ScopedProfileBlock.h"

#define KC_PROFILE_BEGIN(aProfileBlockName)\
    KC_ProfileBlock __profileBlock{ aProfileBlockName };\
    __profileBlock.ResetTime();
#define KC_PROFILE_END()\
    __profileBlock.RecordTime();

#define KC_PROFILE(aProfileBlockName) KC_ScopedProfileBlock __scopedProfileBlock{ aProfileBlockName };
#define KC_PROFILE_STEP_SEPARATOR "::"
#define KC_PROFILE_STEP_TAG(aTask, aStep) aTask KC_PROFILE_STEP_SEPARATOR aStep
#define KC_PROFILE_STEP(aTask, aStep) KC_PROFILE(KC_PROFILE_STEP_TAG(aTask, aStep))

#define KC_PROFILE_GAME KC_PROFILE("Game")
#define KC_PROFILE_GAMERENDERCOPYCOMPONENTS KC_PROFILE_STEP("Game", "RenderCopyComponents")
#define KC_PROFILE_GAMEUPDATE KC_PROFILE_STEP("Game", "Update")
#define KC_PROFILE_RENDER KC_PROFILE("Render")
#define KC_PROFILE_RENDERDRAW KC_PROFILE_STEP("Render", "Draw")

#else
#define KC_PROFILE_BEGIN(aProfileBlockName)
#define KC_PROFILE_END()
#define KC_PROFILE(aProfileBlockName)
#define KC_PROFILE_STEP_SEPARATOR
#define KC_PROFILE_STEP_TAG(aTask, aStep)
#define KC_PROFILE_STEP(aTask, aStep)
#define KC_PROFILE_GAME
#define KC_PROFILE_GAMESERVICEPROVIDER
#define KC_PROFILE_RENDER
#define KC_PROFILE_RENDERDRAW
#endif // IS_DEBUG_BUILD
