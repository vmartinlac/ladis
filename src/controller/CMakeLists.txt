
add_library(
    controller
    SHARED
    Controller.h
    Controller.cpp)

target_link_libraries(controller PUBLIC opencv_core emulator)

