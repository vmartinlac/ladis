
add_executable(
    ladis_example
    main.cpp)

target_link_libraries(ladis_example PUBLIC ladis_checkers opencv_highgui opencv_imgcodecs)

