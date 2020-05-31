#include <LADIS.h>
#include <iostream>
#include "Agent.h"
#include "Checkers.h"

void playWithUser();

int main(int num_args, char** args)
{
    if(true)
    {
        playWithUser();
    }
    else
    {
        DamesAgent agent;
        LADIS::run(&agent, true);
    }

    return 0;
}

void playWithUser()
{
    /*
    const char* initial_grid =
        " o o o o o\n"
        "o o o o o \n"
        " o o o o o\n"
        "o o o o o \n"
        " . . . . .\n"
        ". . . . . \n"
        " p p p p p\n"
        "p p p p p \n"
        " p p p p p\n"
        "p p p p p \n";
    */

    const char* initial_grid =
        " . . . . .\n"
        ". . . . . \n"
        " . . . . .\n"
        ". . . . . \n"
        " . o . . .\n"
        ". p . . . \n"
        " . . . . .\n"
        ". . . . . \n"
        " . . p . .\n"
        ". . . . . \n";

    Checkers::State current_state;
    current_state.setMyTurn(true);
    current_state.setSquareGrid(initial_grid);

    Checkers::Solver solver;

    bool go_on = true;
    while(go_on)
    {
        std::cout << std::endl;
        std::cout << current_state.getSquareGrid() << std::endl;

        if(current_state.isMyTurn())
        {
            std::cout << "YOUR TURN" << std::endl;

            std::vector< std::tuple< Checkers::Action, Checkers::State > > available_actions;

            {
                Checkers::Action available_action;
                Checkers::State available_state;

                Checkers::ActionIterator it;
                it.init(current_state);
                while(it.next(current_state, available_action, available_state))
                {
                    available_actions.emplace_back( available_action, available_state );
                }
            }

            if(available_actions.empty())
            {
                std::cout << "YOU HAVE LOST" << std::endl;
                go_on = false;
            }
            else
            {
                {
                    std::cout << "Your available actions are:" << std::endl;

                    int i = 0;
                    for( const auto& aa : available_actions )
                    {
                        std::cout << "[ " << i << " ] " << std::get<0>(aa).getText() << std::endl;
                        i++;
                    }
                }

                bool keep_asking = true;

                while(keep_asking)
                {
                    int choice;
                    std::cout << "WHICH MOVE?" << std::endl;
                    std::cin >> choice;

                    if(0 <= choice && choice < available_actions.size())
                    {
                        keep_asking = false;
                        current_state = std::get<1>(available_actions[choice]);
                    }
                }
            }
        }
        else
        {
            std::cout << "COMPUTER'S TURN" << std::endl;

            Checkers::Action action;
            Checkers::State next_state;
            go_on = solver.solve(current_state, action, next_state, 10);

            if(go_on)
            {
                current_state = next_state;
            }
            else
            {
                std::cout << "COMPUTER HAS LOST!" << std::endl;
            }
        }
    }
}

