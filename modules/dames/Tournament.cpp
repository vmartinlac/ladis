#include <iostream>
#include <thread>
#include "Tournament.h"

Tournament::Tournament()
{
    myVerbose = true;
    myYield = false;
}

bool Tournament::run(Policy* p1, Policy* p2, int& winner)
{
    bool ret = false;

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

    Checkers::State current_state;
    current_state.setMyTurn(true);
    current_state.setSquareGrid(initial_grid);

    int iteration = 0;
    bool go_on = true;

    while(go_on && iteration < 400)
    {
        Policy* current_player = nullptr;
        Policy* other_player = nullptr;

        if(current_state.isMyTurn())
        {
            current_player = p1;
            other_player = p2;
        }
        else
        {
            current_player = p2;
            other_player = p1;
        }

        if(myVerbose)
        {
            std::cout << "Iteration " << iteration << std::endl;
            std::cout << current_player->getName() << " plays with below grid." << std::endl;
            std::cout << current_state.getSquareGrid() << std::endl;
        }

        auto t0 = std::chrono::high_resolution_clock::now();

        Checkers::Action action;
        Checkers::State next_state;
        go_on = current_player->getAction(current_state, action, next_state);

        auto t1 = std::chrono::high_resolution_clock::now();

        if(go_on)
        {
            current_state = next_state;
            iteration++;

            if(myYield)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        else
        {
            if(myVerbose)
            {
                std::cout << current_player->getName() << " LOST." << std::endl;
                std::cout << other_player->getName() << " WON." << std::endl;
            }

            ret = true;
            winner = (current_state.isMyTurn()) ? 1 : 0;
        }
    }

    return ret;
}

