
add_executable(
    LADIS_dames
    main.cpp
    Minimax.h
    Checkers.h
    Checkers.cpp
    Agent.h
    Agent.cpp)

target_link_libraries(LADIS_dames PUBLIC LADIS_simulator opencv_imgcodecs opencv_imgproc)

