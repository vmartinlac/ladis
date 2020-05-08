
cmake_minimum_required(VERSION 3.1)

project(Ladis)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)
find_package(Threads REQUIRED)

add_subdirectory(modules)

