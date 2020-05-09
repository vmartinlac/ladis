
add_executable(
    LADIS_lecompteestbon
    main.cpp
    Agent.h
    Agent.cpp
    Segmentation.h
    Segmentation.cpp
    UnionFind.h
    Solver.h
    Solver.cpp)

target_link_libraries(LADIS_lecompteestbon PUBLIC LADIS_simulator opencv_imgcodecs opencv_imgproc)

