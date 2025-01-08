cmake_minimum_required(VERSION 3.28)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.6-docking
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG v3.0
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

FetchContent_MakeAvailable(imgui-sfml)

set(KISS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Kiss)

add_library(Kiss STATIC)
target_include_directories(Kiss PUBLIC ${KISS_SOURCE_DIR})
target_sources(Kiss
    PUBLIC
        KC_Assert.h
        KC_CircleRenderer.h
        KC_ComponentArray.h
        KC_ComponentManager.h
        KC_ComponentsRegistry.h
        KC_Defines.h
        KC_DrawCircleSystem.h
        KC_DrawSystem.h
        KC_Entity.h
        KC_EntityManager.h
        KC_EntitySet.h
        KC_Game.h
        KC_GameManager.h
        KC_GameSystemProvider.h
        KC_Manager.h
        KC_ProfileBlock.h
        KC_ProfileBlockTimes.h
        KC_ProfileManager.h
        KC_Profiling.h
        KC_RectangleRenderer.h
        KC_RenderSystemProvider.h
        KC_ScopedProfileBlock.h
        KC_ShapeRenderer.h
        KC_TemplateHelper.h
        KC_ThreadManager.h
        KC_Time.h
        KC_Transform.h
        KC_Velocity.h
        KC_World.h
    PRIVATE
        KC_EntityManager.cpp
        KC_EntitySet.cpp
        KC_DrawCircleSystem.cpp
        KC_GameSystemProvider.cpp
        KC_ProfileBlock.cpp
        KC_ProfileBlockTimes.cpp
        KC_ProfileManager.cpp
        KC_RenderSystemProvider.cpp
        KC_ScopedProfileBlock.cpp
        KC_ThreadManager.cpp
        KC_Time.cpp)
target_link_libraries(Kiss PUBLIC sfml-graphics ImGui-SFML::ImGui-SFML)
target_compile_definitions(Kiss
        PRIVATE
        $<$<BOOL:${IS_DEBUG_BUILD}>:IS_DEBUG_BUILD>
        $<$<BOOL:${IS_IMGUI}>:IS_IMGUI>)
