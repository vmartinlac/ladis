
add_library(
    ladis_controller
    SHARED
    Agent.h
    Agent.cpp
    Controller.h
    Controller.cpp)

target_link_libraries(ladis_controller PUBLIC opencv_core ladis_emulator)

target_include_directories(ladis_controller INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

