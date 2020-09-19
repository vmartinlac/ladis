#include <Utils.h>
#include "UniformAgent.h"

UniformAgent::UniformAgent()
{
    Utils::seedRNG(myRNG);
}

std::string UniformAgent::getName()
{
    return "uniform";
}

void UniformAgent::beginMatch(bool agent_plays_first, int difficulty)
{
}

void UniformAgent::endMatch(int result)
{
}

bool UniformAgent::play(const Checkers::State& state, Checkers::Action& action)
{
    bool ret = false;

    std::vector<Checkers::Action> actions;

    Checkers::enumerateActions(state, actions);

    if(!actions.empty())
    {
        action = actions[myRNG() % actions.size()];
        ret = true;
    }

    return ret;
}

