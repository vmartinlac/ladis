
#pragma once

#include "Policy.h"

class MinimaxPolicy : public Policy
{
public:

    MinimaxPolicy(int max_depth);

    bool getAction(
        const Checkers::State& state_from,
        Checkers::Action& action,
        Checkers::State& state_to) override;

protected:

    int myMaxDepth;
    Checkers::Solver mySolver;
};

