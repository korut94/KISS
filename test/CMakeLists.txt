cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(kiss-test)
target_sources(kiss-test
    PRIVATE
        src/Test.KC_ComponentManager.cpp
        src/Test.KC_EntitySet.cpp
        src/Test.World.cpp
        src/TestHelper.h
        src/TestHelper.cpp
        src/TestHelper/EntitySet/CountMatcher.h
        src/TestHelper/EntitySet/HasEntitiesMatcher.h
        src/TestHelper/EntitySet/HasEntitiesMatcher.cpp
        src/TestHelper/EntitySet/HasIntervalsMatcher.h
        src/TestHelper/EntitySet/HasIntervalsMatcher.cpp)
target_link_libraries(kiss-test PRIVATE Kiss GTest::gmock GTest::gtest_main)
target_compile_definitions(kiss-test PRIVATE IS_DEBUG_BUILD)

include(GoogleTest)
gtest_discover_tests(kiss-test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
