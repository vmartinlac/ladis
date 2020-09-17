
#pragma once

#include <vector>
#include <string>
#include "CheckersBase.h"

class Agent
{
public:

    enum Result
    {
        RESULT_AGENT_WON,
        RESULT_AGENT_LOST,
        RESULT_AGENT_GAVE_UP,
        RESULT_AGENT_ILLEGAL_MOVE,
        RESULT_CONTROLLER_ERROR
    };

    struct State
    {
        char checkerboard[CheckersBase::N];
    };

    struct Action
    {
        std::vector<int> trajectory;
    };

public:

    virtual std::string getName() = 0;
    virtual void beginMatch() = 0;
    virtual bool play(const State& state, Action& move) = 0;
    virtual void endMatch(int result) = 0;
};

