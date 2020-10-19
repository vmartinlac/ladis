
cmake_minimum_required(VERSION 3.1)

project(Ladis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(BUILD_TRAINER "Whether to build training code (requires torch)" OFF)

enable_testing()

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)
find_package(Threads REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
if(BUILD_TRAINER)
    find_package(Torch REQUIRED)
endif()
#find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(src)

