
#pragma once

#include <chrono>
#include <Agent.h>
#include "Database.h"

class SaverAgent : public Agent
{
public:

    SaverAgent(Database* db, Agent* backend);

    std::string getName() final;

    void beginMatch(bool agent_plays_first, int difficulty) final;

    bool play(const Checkers::State& state, Checkers::Action& action) final;

    void endMatch(int result) final;

protected:

    using ClockType = std::chrono::steady_clock;

protected:

    Database* myDatabase;
    Agent* myBackend;
    bool myHasCurrentMatch;
    bool myInterrupted;
    Database::Match myCurrentMatch;
    int myMoveCount;
    ClockType::time_point myStartTime;
};

