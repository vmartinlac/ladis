
add_executable(
    LADIS_dames_test
    main.cpp)

set_target_properties(LADIS_dames_test PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_link_libraries(LADIS_dames_test PUBLIC Qt5::Test LADIS_dames_core opencv_imgcodecs opencv_imgproc)

add_test(NAME dames COMMAND LADIS_dames_test)

