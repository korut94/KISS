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

set(KISS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/kiss)

add_library(kiss STATIC)
target_include_directories(kiss PUBLIC ${KISS_SOURCE_DIR})
target_sources(kiss
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
        KC_RectangleRenderer.h
        KC_RenderSystem.h
        KC_RenderSystemProvider.h
        KC_ShapeRenderer.h
        KC_TemplateHelper.h
        KC_Transform.h
        KC_World.h
    PRIVATE
        KC_EntityManager.cpp
        KC_EntitySet.cpp
        KC_CircleRenderSystem.cpp
        KC_RenderSystemProvider.cpp)
target_link_libraries(kiss PUBLIC sfml-graphics ImGui-SFML::ImGui-SFML)
target_compile_definitions(kiss PRIVATE $<$<CONFIG:Debug>:IS_DEBUG_BUILD>)
