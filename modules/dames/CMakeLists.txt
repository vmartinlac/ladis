
add_executable(
    LADIS_dames
    main.cpp
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
    Policy.cpp
    Agent.h
    Agent.cpp)

target_link_libraries(LADIS_dames PUBLIC LADIS_simulator opencv_imgcodecs opencv_imgproc)

