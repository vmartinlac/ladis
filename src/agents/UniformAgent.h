
#pragma once

#include <Checkers.h>
#include <Agent.h>

class UniformAgent : public Agent
{
public:

    UniformAgent();

    std::string getName() final;

    void beginMatch(bool agent_plays_first, int difficulty) final;

    void endMatch(int result) final;

    bool play(const Checkers::State& state, Checkers::Action& action) final;

protected:

    std::default_random_engine myRNG;
};

