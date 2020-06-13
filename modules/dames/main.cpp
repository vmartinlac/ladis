#include <memory>
#include <random>
#include <fstream>
#include <random>
#include <iostream>
#include <LADIS.h>
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
                        ret = true;
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

    s0.num_consecutive = 0;

    Solver solver;
    const bool ok = solver.solve(s0, a0, s1, 3);

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

void tmp2()
{
    const char* initial_grid =
        " . . . . .\n"
        ". . . o . \n"
        " . . o o .\n"
        "P . . . . \n"
        " . . o o p\n"
        ". . . . . \n"
        " . . . . .\n"
        "p . o . . \n"
        " . . . p p\n"
        ". . p . . \n";

    Checkers::State current_state;
    current_state.setMyTurn(true);
    current_state.setSquareGrid(initial_grid);
    std::cout << current_state.getSquareGrid() << std::endl;

    {
        Checkers::ActionIterator action_iterator;
        action_iterator.init(current_state);

        Checkers::Action action_;
        Checkers::State state_;
        while(action_iterator.next(current_state, action_, state_))
        {
            std::cout << state_.getSquareGrid() << std::endl;
            for(int i=0; i<=action_.getNumMoves(); i++)
            {
                std::cout << action_.getTrajectory(i) << ' ';
            }
            std::cout << std::endl;
        }
    }

    exit(0);
}

void produce_stats()
{
    std::vector< std::shared_ptr<Policy> > policies;

    policies.emplace_back(new UniformPolicy(123));
    policies.back()->setName("UniformPolicy(seed=123)");

    policies.emplace_back(new MinimaxPolicy(3));
    policies.back()->setName("MinimaxPolicy(max_depth=3)");

    policies.emplace_back(new MinimaxPolicy(7));
    policies.back()->setName("MinimaxPolicy(max_depth=7)");

    std::vector< std::tuple<std::shared_ptr<Policy>, std::shared_ptr<Policy> > > pairs;
    pairs.emplace_back( policies[1], policies[1] );
    pairs.emplace_back( policies[1], policies[2] );
    pairs.emplace_back( policies[2], policies[1] );

    std::default_random_engine engine;
    engine.seed(121);

    const int num_samples = pairs.size() * 100;

    Tournament tournament;
    tournament.setVerbose(false);
    tournament.setYield(true);

    std::ofstream logfile("log.txt", std::ofstream::app);

    struct Result
    {
        std::shared_ptr<Policy> policies[2];
        bool ok;
        int winner;
    };

    std::vector<Result> results;
    results.reserve(num_samples);

    while(results.size() < num_samples)
    {
        const int i = engine() % pairs.size();

        Result r;
        r.policies[0] = std::get<0>( pairs[i] );
        r.policies[1] = std::get<1>( pairs[i] );
        
        std::cout << r.policies[0]->getName() << " VS " << r.policies[1]->getName() << std::endl;

        r.ok = tournament.run(r.policies[0].get(), r.policies[1].get(), r.winner);

        results.push_back(r);

        if(r.ok)
        {
            const char* text[2] = { "first", "second" };
            std::cout << "Winner is " << text[r.winner] << ": " << r.policies[r.winner]->getName() << std::endl;
        }
        else
        {
            std::cout << "It is a draw!" << std::endl;
        }

        logfile
            << r.policies[0]->getName() << ' '
            << r.policies[1]->getName() << ' '
            << r.ok << ' '
            << r.winner << std::endl;

        std::cout << results.size() << "/" << num_samples << std::endl;
        std::cout << std::endl;
    }

    logfile.close();
}

int main(int num_args, char** args)
{
    const int depth_min = 2;
    const int depth_max = 9;

    std::default_random_engine rng;
    rng.seed(120);

    std::ofstream logfile("log.csv");

    while(true)
    {
        /*
        const int depth = depth_min + rng() % (depth_max - depth_min + 1);
        const int opponent_skill = rng() % 6;
        */
        const int depth = 2;
        const int opponent_skill = 0;

        DamesAgent agent;
        agent.setSaveScreens(true);
        agent.setOpponentSkill(opponent_skill);
        agent.setMinimaxDepth(depth);

        LADIS::run(&agent, true);

        std::string outcome_string;

        switch(agent.getOutcome())
        {
        case DamesAgent::OUTCOME_WIN:
            outcome_string = "WIN";
            break;
        case DamesAgent::OUTCOME_LOSE:
            outcome_string = "LOSE";
            break;
        case DamesAgent::OUTCOME_DRAW:
            outcome_string = "DRAW";
            break;
        case DamesAgent::OUTCOME_OTHER:
            outcome_string = "OTHER";
            break;
        default:
            throw std::runtime_error("internal error");
        }

        logfile << outcome_string << ' ' << opponent_skill << ' ' << depth << std::endl;
    }

    return 0;
}

