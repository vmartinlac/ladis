
add_executable(
    LADIS_dames_2020
    main.cpp
    Arena.h
    Arena.cpp
    ReinforcementLearningAgent.h
    ReinforcementLearningAgent.cpp
    MinimaxAgent.h
    MinimaxAgent.cpp
    UniformAgent.h
    UniformAgent.cpp
    Agent.h
    Agent.cpp)

target_link_libraries(LADIS_dames_2020 PUBLIC LADIS_simulator LADIS_dames_core opencv_imgcodecs opencv_imgproc)

