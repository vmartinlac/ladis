#include <LADIS.h>
#include <random>
#include <iostream>
#include "Agent.h"
#include "Checkers.h"
#include "Tournament.h"
#include "HumanPolicy.h"
#include "UniformPolicy.h"
#include "MinimaxPolicy.h"

int main(int num_args, char** args)
{
    HumanPolicy human_policy;
    UniformPolicy uniform_policy(123);
    MinimaxPolicy minimax_policy(7);

    if(true)
    {
        Tournament tournament;
        tournament.run(&uniform_policy, &minimax_policy);
        //tournament.run(&minimax_policy, &uniform_policy);
    }
    else
    {
        DamesAgent agent;
        LADIS::run(&agent, true);
    }

    return 0;
}

