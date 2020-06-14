#include <iostream>
#include "HumanPolicy.h"

bool HumanPolicy::getAction(
    const Checkers::State& state_from,
    Checkers::Action& action,
    Checkers::State& state_to)
{
    bool ret = false;

    ActionStateList choices;
    enumerateChoices(state_from, choices);

    //std::cout << std::endl;
    //std::cout << state_from.getSquareGrid() << std::endl;

    if(choices.empty())
    {
        std::cout << "You have lost!" << std::endl;
        ret = false;
    }
    else
    {
        std::cout << "Available choices:" << std::endl;

        {
            int i = 0;
            for( const auto& aa : choices )
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

            if(0 <= choice && choice < choices.size())
            {
                keep_asking = false;
                ret = true;
                action = std::get<0>(choices[choice]);
                state_to = std::get<1>(choices[choice]);
            }
        }
    }

    return ret;
}

