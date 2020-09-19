
#pragma once

#include <ctime>
#include <vector>
#include "Checkers.h"

class Database
{
public:

    struct AgentMove
    {
        Checkers::State state;
        Checkers::Action action;
        double time_offset;
        double computation_time;
    };

    struct Match
    {
        time_t start_timestamp;
        bool agent_plays_first;
        int difficulty;
        int result;
        std::string agent;
        std::vector<AgentMove> agent_moves;
    };

    virtual void saveMatch(const Match& match) = 0;
};

