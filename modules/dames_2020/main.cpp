#include <memory>
#include <random>
#include <fstream>
#include <random>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include "Arena.h"
#include "MinimaxAgent.h"
#include "UniformAgent.h"
#include "StateSequence.h"
#include "EpsilonGreedyAgent.h"

void generateEnvironmentSamples()
{
    RNGPtr rng = std::make_shared<RNG>();

    Arena& arena = Arena::getInstance();

    arena.start();

    MinimaxAgent agent0;
    agent0.setMaxDepth(5);

    UniformAgent agent1;
    agent1.setRNG(rng);

    EpsilonGreedyAgent agent2;
    agent2.setEpsilon(0.1);
    agent2.setGreedyAgent(&agent0);
    agent2.setRandomAgent(&agent1);
    agent2.setRNG(rng);

    std::vector<Agent*> agents{&agent0, &agent1, &agent2};

    int file_count = 0;

    while(true)
    {
        const int opponent_skill = 0;
        //const bool agent_starts = true;
        const bool agent_starts( (*rng)() % 2 );
        //const int agent_index = 1;
        const int agent_index = (*rng)() % agents.size();

        arena.setAgentStarts(agent_starts);
        arena.setOpponentSkill(opponent_skill);
        arena.play(agents[agent_index]);

        std::string result;

        switch(arena.getOutcome())
        {
        case Arena::OUTCOME_WIN:
            result = "win";
            break;
        case Arena::OUTCOME_LOSE:
            result = "lose";
            break;
        case Arena::OUTCOME_DRAW:
            result = "draw";
            break;
        case Arena::OUTCOME_ILLEGAL_AGENT_MOVE:
            result = "illegal_agent_move";
            break;
        case Arena::OUTCOME_ILLEGAL_OPPONENT_MOVE:
            result = "illegal_opponent_move";
            break;
        case Arena::OUTCOME_ERROR:
        default:
            throw std::runtime_error("internal error");
        }

        std::cout << "GAME ENDED: " << result << std::endl;

        {
            std::string metadata_fname;
            std::string log_fname;

            bool go_on = true;
            while(go_on)
            {
                metadata_fname = "metadata_" + std::to_string(file_count) + ".txt";
                log_fname = "log_" + std::to_string(file_count) + ".txt";
                file_count++;

                struct stat stats;
                const int r0 = stat(metadata_fname.c_str(), &stats);
                const int r1 = stat(log_fname.c_str(), &stats);

                go_on = !( bool(r0) || bool(r1) );
            }

            std::ofstream f(metadata_fname.c_str());
            f << opponent_skill << std::endl;
            f << agent_starts << std::endl;
            f << agent_index << std::endl;
            f << result << std::endl;
            f.close();

            StateSequence::save(arena.refLog(), log_fname.c_str());
        }
    }

    arena.quit();
}

void evaluateModel()
{
    // compare how well the model predicts DA2020 moves.
}

void trainReinforcementLearningAgentAgainstMinimax()
{
}

void learnDA2020Model()
{
}

void trainReinforcementLearningAgentAgainstDA2020()
{
}

////////

/*
class UtilityFunctionGeneticOptimizer
{
public:

    UtilityFunctionGeneticOptimizer()
    {
        myPopulationSize = 500;
        myNumGenerations = 20;
        myMaxDepth = 5;
        myMaxNumTurns = 250;
        myDeathRate = 0.5;
        myEngine.seed(1234);
    }

    void run(Checkers::UtilityFunction& best)
    {
        std::vector<Individual> population(myPopulationSize);

        for(int i=0; i<myPopulationSize; i++)
        {
            population[i].num_victories = 0;
        }

        for(int generation=0; generation<myNumGenerations; generation++)
        {
            for(int i=0; i<myPopulationSize; i++)
            {
                population[i].num_victories = 0;
            }

            // evaluate fitness.

            for(int i=0; i<myPopulationSize; i++)
            {
                for(int j=0; j<myPopulationSize; j++)
                {
                    if(i != j)
                    {
                        fight(population[i], population[j]);
                    }
                }
            }

            // select and reproduce best individuals.

            for(int i=0; i<int(myDeathRate*myPopulationSize); i++)
            {
                //myPopulationSize[i].utility;
            }
        }
    }

protected:

    struct Individual
    {
        Individual()
        {
            num_victories = 0;
            utility.setDefaultWeights();
        }

        Checkers::UtilityFunction utility;
        int num_victories;
    };

protected:

    void fight(Individual& player0, Individual& player1)
    {
        Checkers::State current_state;
        current_state.setSquareGrid(
            " o o o o o\n"
            "o o o o o \n"
            " o o o o o\n"
            "o o o o o \n"
            " . . . . .\n"
            ". . . . . \n"
            " p p p p p\n"
            "p p p p p \n"
            " p p p p p\n"
            "p p p p p \n");
        current_state.setMyTurn(true);

        bool go_on = true;
        int num_turns = 0;

        while(go_on)
        {
            if(current_state.isMyTurn())
            {
                Checkers::State new_state;
                Checkers::Action action;

                const bool ok = mySolver.solve(current_state, action, new_state, player0.utility, myMaxDepth);

                if(ok)
                {
                    current_state = new_state;
                }
                else
                {
                    player1.num_victories += 2;
                    go_on = false;
                }
            }
            else
            {
                current_state.invert();

                Checkers::State new_state;
                Checkers::Action action;

                const bool ok = mySolver.solve(current_state, action, new_state, player1.utility, myMaxDepth);

                if(ok)
                {
                    current_state = new_state;
                    current_state.invert();
                }
                else
                {
                    player0.num_victories += 2;
                }
            }

            num_turns++;

            if(num_turns > myMaxNumTurns)
            {
                player0.num_victories++;
                player1.num_victories++;
                go_on = false;
            }
        }
    }

protected:

    Checkers::Solver mySolver;
    int myPopulationSize;
    int myNumGenerations;
    int myMaxDepth;
    int myMaxNumTurns;
    float myDeathRate;
    std::default_random_engine myEngine;
};
*/

int main(int num_args, char** args)
{
    generateEnvironmentSamples();

    return 0;
}

