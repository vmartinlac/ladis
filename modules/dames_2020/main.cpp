#include <memory>
#include <random>
#include <fstream>
#include <random>
#include <iostream>
#include "Arena.h"
#include "MinimaxAgent.h"
#include "UniformAgent.h"

void trainReinforcementLearningAgentAgainstMinimax()
{
    ;
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
    Arena& arena = Arena::getInstance();

    arena.start();

    MinimaxAgent agent;
    agent.setMaxDepth(4);

    UniformAgent agent2;

    arena.setOpponentSkill(0);
    while(true)
    {
        arena.play(&agent2);

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
            result = "error";
            break;
        default:
            throw std::runtime_error("internal error");
        }

        std::cout << "GAME ENDED: " << result << std::endl;
    }

    arena.quit();

    return 0;
}

