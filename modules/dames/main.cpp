#include <LADIS.h>
#include <iostream>
#include "Agent.h"
#include "Checkers.h"

void playWithUser();
void test1();
void test2();

int main(int num_args, char** args)
{
    if(false)
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

void test2()
{
    const char* initial_grid =
        " . . . . .\n"
        ". . . . . \n"
        " . o . . .\n"
        ". p . . . \n"
        " . . . . .\n"
        ". . . . O \n"
        " . . . . .\n"
        ". o . . . \n"
        " p . . . .\n"
        ". . . . . \n";

    Checkers::State current_state;
    current_state.setMyTurn(true);
    current_state.setSquareGrid(initial_grid);

    Checkers::Solver solver;
    Checkers::Action action;
    Checkers::State next_state;
    const bool ok = solver.solve(current_state, action, next_state, 10);
    if(ok)
    {
        std::cout << current_state.getSquareGrid() << std::endl;
        std::cout << std::endl;
        std::cout << next_state.getSquareGrid() << std::endl;
    }
}

void test1()
{
    const char* initial_grid =
        " . . . . .\n"
        ". . . . . \n"
        " . o . . .\n"
        ". p . . . \n"
        " . . . . .\n"
        ". . . . O \n"
        " . . . . .\n"
        ". . o . . \n"
        " . p . . .\n"
        ". . . . . \n";

    Checkers::State current_state;
    current_state.setMyTurn(true);
    current_state.setSquareGrid(initial_grid);
    std::cout << current_state.getSquareGrid() << std::endl;

    Checkers::ActionIterator it;
    it.init(current_state);

    Checkers::State s;
    Checkers::Action a;
    while(it.next(current_state, a, s))
    {
        for(int i=0; i<=a.getNumMoves(); i++)
        {
            std::cout << a.getTrajectory(i) << ' ';
        }
        std::cout << std::endl;
        std::cout << s.getSquareGrid() << std::endl;
        std::cout << std::endl;
    }
}

void playWithUser()
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

    Checkers::Solver solver;

    bool go_on = true;
    while(go_on)
    {
        std::cout << std::endl;
        std::cout << "####################################" << std::endl;
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

            current_state.invert();

            Checkers::Action action;
            Checkers::State next_state;
            go_on = solver.solve(current_state, action, next_state, 11);

            /*
            Checkers::ActionIterator it;
            it.init(current_state);
            Checkers::Action aa;
            Checkers::State ss;
            while(it.next(current_state, aa, ss)) std::cout << "A" << std::endl;
            */

            if(go_on)
            {
                current_state = next_state;
                current_state.invert();
            }
            else
            {
                std::cout << "COMPUTER HAS LOST!" << std::endl;
            }
        }
    }
}

