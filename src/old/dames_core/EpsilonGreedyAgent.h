
#pragma once

#include "Agent.h"
#include "RNG.h"

class EpsilonGreedyAgent : public Agent
{
public:

    EpsilonGreedyAgent();

    void setRNG(RNGPtr rng);
    void setEpsilon(double epsilon);
    void setGreedyAgent(Agent* a);
    void setRandomAgent(Agent* a);

    void beforeEpisode() override;
    void afterEpisose() override;
    bool getAction(const Checkers::State& s, Checkers::Action& a) override;

protected:

    Agent* myRandomAgent;
    Agent* myGreedyAgent;
    double myEpsilon;
    RNGPtr myRNG;
    std::uniform_real_distribution<double> myU;
};

