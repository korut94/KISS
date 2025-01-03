cmake_minimum_required(VERSION 3.28)

set(KISS_EDITOR_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/KissEditor)

add_library(KissEditor STATIC)
target_include_directories(KissEditor PUBLIC ${KISS_EDITOR_SOURCE_DIR})
target_sources(KissEditor
    PUBLIC
        KT_Editor.h)

target_link_libraries(KissEditor PUBLIC Kiss)
target_compile_definitions(KissEditor PRIVATE IS_DEBUG_BUILD)
