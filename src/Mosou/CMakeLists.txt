cmake_minimum_required(VERSION 3.28)

set(MOSOU_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Mosou)

add_library(Mosou STATIC)
target_include_directories(Mosou PUBLIC ${MOSOU_SOURCE_DIR})
target_sources(Mosou
    PUBLIC
        MC_Game.h
    PRIVATE
        MC_Game.cpp)

target_link_libraries(Mosou PUBLIC Kiss)
target_compile_definitions(Mosou PRIVATE $<$<CONFIG:Debug>:IS_DEBUG_BUILD>)
