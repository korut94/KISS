cmake_minimum_required(VERSION 3.28)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
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
  GIT_TAG 2.6.x
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
        KC_CircleRenderSystem.h
        KC_ComponentArray.h
        KC_ComponentManager.h
        KC_ComponentsRegistry.h
        KC_Entity.h
        KC_EntityManager.h
        KC_EntitySet.h
        KC_Game.h
        KC_GameManager.h
        KC_Profiler.h
        KC_ProfileTimerTypes.h
        KC_Profiling.h
        KC_RectangleRenderer.h
        KC_RenderSystem.h
        KC_RenderSystemProvider.h
        KC_ScopedProfileTimer.h
        KC_ShapeRenderer.h
        KC_TemplateHelper.h
        KC_Transform.h
        KC_World.h
    PRIVATE
        KC_EntityManager.cpp
        KC_EntitySet.cpp
        KC_CircleRenderSystem.cpp
        KC_Profiler.cpp
        KC_RenderSystemProvider.cpp
        KC_ScopedProfileTimer.cpp)
target_link_libraries(Kiss PUBLIC sfml-graphics ImGui-SFML::ImGui-SFML)
target_compile_definitions(Kiss
        PRIVATE
        $<$<BOOL:${IS_DEBUG_BUILD}>:IS_DEBUG_BUILD>
        $<$<BOOL:${IS_IMGUI}>:IS_IMGUI>)
