#include <Utils.h>
#include "ExperimentalAgent.h"

ExperimentalAgent::ExperimentalAgent()
{
    Utils::seedRNG(myRNG);
}

std::string ExperimentalAgent::getName()
{
    return "experimental";
}

void ExperimentalAgent::beginMatch(bool agent_plays_first, int difficulty)
{
}

void ExperimentalAgent::endMatch(int result)
{
}

bool ExperimentalAgent::play(const Checkers::State& state, Checkers::Action& action)
{
    bool ret = false;

    return ret;
}

