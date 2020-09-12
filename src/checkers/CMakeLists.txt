
add_library(
    ladis_checkers
    SHARED
    Checkers.cpp
    Checkers.h
    GraphVizHook.cpp
    GraphVizHook.h
    Minimax.h
    StateSequence.cpp
    StateSequence.h)

target_link_libraries(ladis_checkers PUBLIC opencv_imgcodecs opencv_imgproc)

target_include_directories(ladis_checkers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

