#include "UniformAgent.h"

UniformAgent::UniformAgent()
{
}

void UniformAgent::setRNG(RNGPtr rng)
{
    myRNG = std::move(rng);
}

void UniformAgent::beforeEpisode()
{
}

void UniformAgent::afterEpisose()
{
}

bool UniformAgent::getAction(const Checkers::State& s, Checkers::Action& a)
{
    bool ret = false;

    Checkers::ActionIterator it;
    Checkers::Action action_;
    Checkers::State state_;

    it.init(s);

    myActions.clear();
    while(it.next(s, action_, state_))
    {
        myActions.push_back(action_);
    }

    if(!myActions.empty())
    {
        a = myActions[(*myRNG)() % myActions.size()];
        ret = true;
    }

    return ret;
}

