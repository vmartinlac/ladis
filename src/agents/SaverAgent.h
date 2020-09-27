
#pragma once

#include <chrono>
#include <Agent.h>
#include "MatchLog.h"

class SaverAgent : public Agent
{
public:

    SaverAgent(Agent* backend);

    std::string getName() final;

    void beginMatch(bool agent_plays_first, int difficulty) final;

    bool play(const Checkers::State& state, Checkers::Action& action) final;

    void endMatch(int result) final;

    MatchLogPtr retrieveMatchLog();

protected:

    using ClockType = std::chrono::system_clock;

protected:

    Agent* myBackend;
    bool myInterrupted;
    MatchLogPtr myCurrentMatch;
    ClockType::time_point myStartTime;
};

