cmake_minimum_required(VERSION 3.28)

set(MUSOU_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Musou)

add_library(Musou STATIC)
target_include_directories(Musou PUBLIC ${MUSOU_SOURCE_DIR})
target_sources(Musou
    PUBLIC
        MC_Game.h
        MC_BounceOnCollisionSystem.h
        MC_MoveSystem.h
    PRIVATE
        MC_Game.cpp
        MC_BounceOnCollisionSystem.cpp
        MC_MoveSystem.cpp)

target_link_libraries(Musou PUBLIC Kiss)
target_compile_definitions(Musou
        PRIVATE
        $<$<BOOL:${IS_DEBUG_BUILD}>:IS_DEBUG_BUILD>
        $<$<BOOL:${IS_IMGUI}>:IS_IMGUI>)
