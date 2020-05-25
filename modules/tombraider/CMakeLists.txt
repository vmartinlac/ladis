
add_executable(LADIS_tombraider main.cpp Agent.h Agent.cpp)
target_link_libraries(LADIS_tombraider PUBLIC LADIS_simulator opencv_imgcodecs)

