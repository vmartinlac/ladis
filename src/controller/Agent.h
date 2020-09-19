
#pragma once

#include <vector>
#include <string>
#include "Checkers.h"

class Agent
{
public:

    enum Result
    {
        RESULT_AGENT_WON,
        RESULT_AGENT_LOST,
        RESULT_AGENT_GAVE_UP,
        RESULT_AGENT_ILLEGAL_MOVE,
        RESULT_CONTROLLER_ERROR,
        RESULT_AGENT_ERROR
    };

public:

    virtual std::string getName() = 0;
    virtual void beginMatch(bool agent_plays_first, int difficulty) = 0;
    virtual bool play(const Checkers::State& state, Checkers::Action& action) = 0;
    virtual void endMatch(int result) = 0;
};

