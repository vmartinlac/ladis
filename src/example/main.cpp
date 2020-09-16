#include <iostream>
#include <opencv2/highgui.hpp>
#include <thread>
#include "Emulator.h"
#include "Controller.h"
#include "Checkers.h"

class CheckersAgent : public Agent
{
public:

    void beginMatch() override
    {
    }

    bool play(const State& state_, Action& action_) override
    {
        Checkers::Solver solver;
        Checkers::State state;
        Checkers::State new_state;
        Checkers::Action action;
        Checkers::UtilityFunction utility;

        state.setMyTurn(true);
        state.setFlatGrid(state_.checkerboard);

        const bool solver_ok = solver.solve(state, action, new_state, utility, 4);

        if(solver_ok)
        {
            action_.trajectory.resize(action.getNumMoves()+1);

            for(int i=0; i<=action.getNumMoves(); i++)
            {
                action_.trajectory[i] = action.getTrajectory(i);
                std::cout << action.getTrajectory(i) << ' ';
            }
            std::cout << std::endl;
        }

        return solver_ok;
    }

    void endMatch(int result) override
    {
        std::cout << "result: " << result << std::endl;
    }
};

class MyAgent : public CheckersAgent
{
};

int main(int num_args, char** args)
{
    Emulator em;
    MyAgent agent;
    Controller con;

    con.run(&em, &agent, true, 0);

    return 0;
}

