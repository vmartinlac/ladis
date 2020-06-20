
#pragma once

#include <random>
#include "Agent.h"
#include "RNG.h"

class UniformAgent : public Agent
{
public:

    UniformAgent();

    void setRNG(RNGPtr rng);

    void beforeEpisode() override;
    void afterEpisose() override;
    bool getAction(const Checkers::State& s, Checkers::Action& a) override;

protected:

    RNGPtr myRNG;
    std::vector<Checkers::Action> myActions;
};

