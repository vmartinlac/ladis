
#pragma once

#include <memory>
#include <ctime>
#include <vector>
#include "Checkers.h"

struct MatchLog
{
    struct AgentMove
    {
        Checkers::State state;
        Checkers::Action action;
        double time_offset;
        double computation_time;
    };

    time_t start_timestamp;
    bool agent_plays_first;
    int difficulty;
    int result;
    std::string agent;
    std::vector<AgentMove> agent_moves;

    void saveJson(const std::string& path);
};

using MatchLogPtr = std::shared_ptr<MatchLog>;

