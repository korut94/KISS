cmake_minimum_required(VERSION 3.28)
project(MosouDemo LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

enable_testing()

add_subdirectory(src/Kiss)
add_subdirectory(src/KissEditor)
add_subdirectory(src/Mosou)
add_subdirectory(test)

add_executable(mosou-demo src/main.cpp)
target_link_libraries(mosou-demo PRIVATE KissEditor Mosou)
target_compile_features(mosou-demo PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET mosou-demo
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:mosou-demo>
        VERBATIM)
endif()
