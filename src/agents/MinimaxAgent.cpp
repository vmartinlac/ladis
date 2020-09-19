#include "MinimaxAgent.h"

std::string MinimaxAgent::getName()
{
    return "minimax";
}

bool MinimaxAgent::play(const Checkers::State& state, Checkers::Action& action)
{
    Checkers::Solver solver;
    Checkers::State new_state;
    Checkers::UtilityFunction utility;

    return solver.solve(state, action, new_state, utility, 4);
}

void MinimaxAgent::beginMatch(bool agent_plays_first, int difficulty)
{
}

void MinimaxAgent::endMatch(int result)
{
}

