add_executable(experiment-sfmlimgui main.cpp)
target_link_libraries(experiment-sfmlimgui PRIVATE Kiss)
target_compile_features(experiment-sfmlimgui PRIVATE cxx_std_17)
