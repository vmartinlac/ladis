
add_library(
    LADIS_dames_core
    SHARED
    Minimax.h
    Checkers.h
    Checkers.cpp
    MinimaxPolicy.h
    MinimaxPolicy.cpp
    UniformPolicy.h
    UniformPolicy.cpp
    HumanPolicy.h
    HumanPolicy.cpp
    Tournament.h
    Tournament.cpp
    Policy.h
    Policy.cpp)

target_link_libraries(LADIS_dames_core PUBLIC opencv_imgcodecs opencv_imgproc)

target_include_directories(LADIS_dames_core INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

