
#pragma once

#include <Agent.h>

class MinimaxAgent : public Agent
{
public:

    std::string getName() final;

    bool play(const Checkers::State& state, Checkers::Action& action) final;

    void beginMatch(bool agent_plays_first, int difficulty) final;

    void endMatch(int result) final;
};

