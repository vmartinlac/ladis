
add_library(
    LADIS_dames_core
    SHARED
    Minimax.h
    Checkers.h
    Checkers.cpp
    RNG.h
    RNG.cpp
    GraphVizHook.h
    GraphVizHook.cpp
    ReinforcementLearningAgent.h
    ReinforcementLearningAgent.cpp
    MinimaxAgent.h
    MinimaxAgent.cpp
    UniformAgent.h
    UniformAgent.cpp
    RNG.h
    EpsilonGreedyAgent.h
    EpsilonGreedyAgent.cpp
    StateSequence.h
    StateSequence.cpp
    Model.h
    Model.cpp
    ModelBasedAgent.h
    ModelBasedAgent.cpp
    Agent.h
    Agent.cpp)

target_link_libraries(LADIS_dames_core PUBLIC opencv_imgcodecs opencv_imgproc Qt5::Core torch)

target_include_directories(LADIS_dames_core INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

