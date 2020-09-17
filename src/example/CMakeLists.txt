
add_executable(
    ladis_example
    Database.h
    Database.cpp
    main.cpp)

target_link_libraries(ladis_example PUBLIC ladis_checkers opencv_highgui opencv_imgcodecs PkgConfig::SQLITE3)

