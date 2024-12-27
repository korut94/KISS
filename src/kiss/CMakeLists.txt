cmake_minimum_required(VERSION 3.28)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(kiss STATIC)
target_include_directories(kiss PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
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
        KC_GameManager.h
        KC_RectangleRenderer.h
        KC_RenderSystem.h
        KC_RenderSystemProvider.h
        KC_RenderThread.h
        KC_ShapeRenderer.h
        KC_TemplateHelper.h
        KC_Transform.h
        KC_World.h
    PRIVATE
        KC_EntityManager.cpp
        KC_EntitySet.cpp
        KC_GameManager.cpp
        KC_CircleRenderSystem.cpp
        KC_RenderThread.cpp)
target_link_libraries(kiss PUBLIC sfml-graphics)
target_compile_definitions(kiss PRIVATE $<$<CONFIG:Debug>:IS_DEBUG_BUILD>)
