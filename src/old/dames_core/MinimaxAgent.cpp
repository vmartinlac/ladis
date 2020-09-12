#include "MinimaxAgent.h"

MinimaxAgent::MinimaxAgent()
{
    myUtility.setDefaultWeights();
    myMaxDepth = 4;
}

void MinimaxAgent::beforeEpisode()
{
}

void MinimaxAgent::afterEpisose()
{
}

bool MinimaxAgent::getAction(const Checkers::State& s, Checkers::Action& a)
{
    Checkers::State resulting_state;

    const bool ok = mySolver.solve(s, a, resulting_state, myUtility, myMaxDepth);

    return ok;
}

void MinimaxAgent::setMaxDepth(int max_depth)
{
    myMaxDepth = max_depth;
}

