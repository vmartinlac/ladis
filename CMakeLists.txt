
cmake_minimum_required(VERSION 3.1)

project(Ladis)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)
find_package(Threads REQUIRED)

add_subdirectory(modules)

