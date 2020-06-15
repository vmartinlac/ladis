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
    if(num_args != 3)
    {
        throw std::runtime_error("bad command line!");
    }

    const int depth = atoi(args[1]);
    const int opponent_skill = atoi(args[2]);

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

    std::cout << "LADIS_RESULT " << outcome_string << ' ' << opponent_skill << ' ' << depth << std::endl;

    return 0;
}
