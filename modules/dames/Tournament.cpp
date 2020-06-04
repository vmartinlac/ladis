#include <iostream>
#include "Tournament.h"

void Tournament::run(Policy* p1, Policy* p2)
{
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

    struct Player
    {
        Policy* policy;
        std::string name;
    };

    Player player1;
    player1.policy = p1;
    player1.name = "Player I";

    Player player2;
    player2.policy = p2;
    player2.name = "Player II";

    Player* current_player = &player1;
    Player* next_player = &player2;

    int iteration = 0;
    bool go_on = true;

    while(go_on)
    {
        std::cout << "Iteration " << iteration << std::endl;
        std::cout << current_player->name << " plays with below grid." << std::endl;
        std::cout << current_state.getSquareGrid() << std::endl;

        Checkers::Action action;
        Checkers::State next_state;
        go_on = current_player->policy->getAction(current_state, action, next_state);

        if(go_on)
        {
            current_state = next_state;
            std::swap(current_player, next_player);
            iteration++;
        }
        else
        {
            std::cout << current_player->name << " LOST." << std::endl;
            std::cout << next_player->name << " WON." << std::endl;
        }
    }
}

