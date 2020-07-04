
add_executable(
    LADIS_dames_train
    LogLoader.h
    LogLoader.cpp
    ActionDataset.h
    ActionDataset.cpp
    DatasetSubset.h
    DatasetSubset.cpp
    main.cpp)

target_link_libraries(LADIS_dames_train PUBLIC torch LADIS_dames_core opencv_imgcodecs opencv_imgproc Qt5::Core)

#set_target_properties(LADIS_dames_train PROPERTIES CXX_STANDARD 14)

