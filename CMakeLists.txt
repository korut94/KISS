cmake_minimum_required(VERSION 3.28)
project(MusouDemo LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

enable_testing()

add_subdirectory(src/Kiss)
add_subdirectory(src/KissEditor)
add_subdirectory(src/Musou)
add_subdirectory(experiments)
add_subdirectory(test)

if(${IS_EDITOR_BUILD})
	add_executable(musou-editor src/main.cpp)
	target_link_libraries(musou-editor PRIVATE KissEditor Musou)
	target_compile_features(musou-editor PRIVATE cxx_std_17)
	target_compile_definitions(musou-editor
		PRIVATE
		$<$<BOOL:${IS_DEBUG_BUILD}>:IS_DEBUG_BUILD>
		$<$<BOOL:${IS_EDITOR_BUILD}>:IS_EDITOR_BUILD>
		$<$<BOOL:${IS_IMGUI}>:IS_IMGUI>)
else()
	add_executable(musou-demo src/main.cpp)
	target_link_libraries(musou-demo PRIVATE Musou)
	target_compile_features(musou-demo PRIVATE cxx_std_17)
	target_compile_definitions(musou-demo
		PRIVATE
		$<$<BOOL:${IS_DEBUG_BUILD}>:IS_DEBUG_BUILD>
		$<$<BOOL:${IS_IMGUI}>:IS_IMGUI>)
endif()
