#include "MinimaxPolicy.h"

MinimaxPolicy::MinimaxPolicy(int max_depth)
{
    myMaxDepth = max_depth;
    myUtility.setDefaultWeights();
}

bool MinimaxPolicy::getAction(
    const Checkers::State& state_from,
    Checkers::Action& action,
    Checkers::State& state_to)
{
    bool ret = false;

    Checkers::State state;

    state = state_from;

    if(!state_from.isMyTurn())
    {
        state.invert();
    }

    ret = mySolver.solve(state, action, state_to, myUtility, myMaxDepth);

    if(ret)
    {
        if(!state_from.isMyTurn())
        {
            action.invert();
            state_to.invert();
        }
    }

    return ret;
}

