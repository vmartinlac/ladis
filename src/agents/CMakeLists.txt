
add_library(
    ladis_agents
    SHARED
    SaverAgent.h
    SaverAgent.cpp
    MinimaxAgent.h
    MinimaxAgent.cpp
    UniformAgent.h
    UniformAgent.cpp
    MySQLDatabase.h
    MySQLDatabase.cpp
    Database.h
    Database.cpp)

target_link_libraries(ladis_agents PUBLIC ladis_controller PkgConfig::MARIADB PkgConfig::JSONCPP)

target_include_directories(ladis_agents INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

