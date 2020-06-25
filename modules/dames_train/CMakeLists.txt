
add_executable(
    LADIS_dames_train
    ActionDataset.h
    ActionDataset.cpp
    main.cpp)

target_link_libraries(LADIS_dames_train PUBLIC torch LADIS_dames_core opencv_imgcodecs opencv_imgproc Qt5::Core)

#set_target_properties(LADIS_dames_train PROPERTIES CXX_STANDARD 14)

