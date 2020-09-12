
cmake_minimum_required(VERSION 3.1)

project(Ladis)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

enable_testing()

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Test)
find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)
find_package(Threads REQUIRED)
find_package(Torch REQUIRED)

add_subdirectory(src)

