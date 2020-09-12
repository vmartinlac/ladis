
add_executable(
    example
    main.cpp)

target_link_libraries(example PUBLIC emulator opencv_highgui opencv_imgcodecs)

