#include "EpsilonGreedyAgent.h"

EpsilonGreedyAgent::EpsilonGreedyAgent() : myU(0.0, 1.0)
{
    myRandomAgent = nullptr;
    myGreedyAgent = nullptr;
    myEpsilon = 0.05;
}

void EpsilonGreedyAgent::setEpsilon(double epsilon)
{
    myEpsilon = epsilon;
}

void EpsilonGreedyAgent::setRNG(RNGPtr rng)
{
    myRNG = rng;
}

void EpsilonGreedyAgent::setGreedyAgent(Agent* a)
{
    myGreedyAgent = a;
}

void EpsilonGreedyAgent::setRandomAgent(Agent* a)
{
    myRandomAgent = a;
}

void EpsilonGreedyAgent::beforeEpisode()
{
    myGreedyAgent->beforeEpisode();
    myRandomAgent->beforeEpisode();
}

void EpsilonGreedyAgent::afterEpisose()
{
    myGreedyAgent->afterEpisose();
    myRandomAgent->afterEpisose();
}

bool EpsilonGreedyAgent::getAction(const Checkers::State& s, Checkers::Action& a)
{
    const double alpha = myU(*myRNG);

    bool ret = false;

    if(alpha < myEpsilon)
    {
        ret = myRandomAgent->getAction(s, a);
    }
    else
    {
        ret = myGreedyAgent->getAction(s, a);
    }

    return ret;
}

