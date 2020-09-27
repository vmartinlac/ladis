
add_library(
    ladis_agents
    SHARED
    MatchLog.h
    MatchLog.cpp
    SaverAgent.h
    SaverAgent.cpp
    MinimaxAgent.h
    MinimaxAgent.cpp
    UniformAgent.h
    UniformAgent.cpp
    ExperimentalAgent.h
    ExperimentalAgent.cpp)

target_link_libraries(ladis_agents PUBLIC ladis_controller mongo::mongocxx_shared mongo::bsoncxx_shared)

target_include_directories(ladis_agents INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

