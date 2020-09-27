
#pragma once

#include <map>
#include <Checkers.h>
#include <Agent.h>

class ExperimentalAgent : public Agent
{
public:

    ExperimentalAgent();

    std::string getName() final;

    void beginMatch(bool agent_plays_first, int difficulty) final;

    void endMatch(int result) final;

    bool play(const Checkers::State& state, Checkers::Action& action) final;

protected:

    std::default_random_engine myRNG;
    std::multimap<Checkers::Grid,Checkers::Grid> myOpponentKnownTransitions;
};

