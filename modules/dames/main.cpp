#include <LADIS.h>
#include <random>
#include <iostream>
#include "Agent.h"
#include "Checkers.h"
#include "Tournament.h"
#include "HumanPolicy.h"
#include "UniformPolicy.h"
#include "MinimaxPolicy.h"

void tmp()
{
    struct State
    {
        float getValue() const
        {
            return score;
        }

        bool isMyTurn() const
        {
            return (time % 2) == 0;
        }

        State()
        {
            time = 0;
            score = 0.0f;
            num_consecutive = 0;
        }

        void print()
        {
            std::cout << "State( time=" << time << ", score=" << score << ", num_consecutive=" << num_consecutive << " )" << std::endl;
        }

        int time;
        float score;
        int num_consecutive;
    };

    struct Action
    {
    };

    struct ActionIterator
    {
        void init(const State& s)
        {
            my_next = 0;
        }

        bool next(const State& state_from, Action& action, State& state_to)
        {
            bool ret = false;

            if(state_from.time < 100)
            {
                if(state_from.isMyTurn())
                {
                    if(my_next <= 2)
                    {
                        ret = true;
                        state_to.score = state_from.score + my_next-1;
                        if(my_next-1 >= 0)
                        {
                            state_to.num_consecutive = state_from.num_consecutive+1;
                        }
                        else
                        {
                            state_to.num_consecutive = 0;
                        }
                        state_to.time = state_from.time + 1;
                        my_next++;
                    }
                }
                else
                {
                    const float delta = (state_from.num_consecutive >= 2) ? 5 : 1;

                    if(my_next <= 1)
                    {
                        state_to.time = state_from.time + 1;
                        state_to.num_consecutive = state_from.num_consecutive;
                        state_to.score = state_from.score + delta * ( (my_next == 0) ? 1.0f : -1.0f );
                        my_next++;
                    }
                }
            }

            return ret;
        }

        int my_next;
    };

    using Solver = Minimax<State, Action, ActionIterator>;

    State s0;
    Action a0;
    State s1;

    s0.num_consecutive = 2;

    Solver solver;
    const bool ok = solver.solve(s0, a0, s1, 10);

    if(ok)
    {
        s1.print();
    }
    else
    {
        std::cout << "Solver returned false!" << std::endl;
    }

    exit(0);
}

int main(int num_args, char** args)
{
    tmp();

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

