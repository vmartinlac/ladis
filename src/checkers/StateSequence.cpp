#include <iostream>
#include <fstream>
#include "StateSequence.h"

bool StateSequence::load(const std::string& path, std::vector<Checkers::State>& sequence)
{
    std::ifstream f(path);
    bool ok = true;

    ok = f.is_open();

    sequence.clear();

    if(ok)
    {
        Checkers::State s;
        char buffer[Checkers::N+2];

        f.read(buffer, Checkers::N+2);
        while(f.good())
        {
            s.setMyTurn(buffer[0] == '0');
            
            buffer[Checkers::N+1] = 0;
            s.setFlatGrid(buffer+1);

            sequence.push_back(s);

            f.read(buffer, Checkers::N+2);
        }
    }

    return ok;
}

bool StateSequence::save(const std::vector<Checkers::State>& sequence, const std::string& path)
{
    std::ofstream f(path);
    bool ok = true;

    ok = f.is_open();

    if(ok)
    {
        for(const Checkers::State& s : sequence)
        {
            if(s.isMyTurn())
            {
                f << '0';
            }
            else
            {
                f << '1';
            }

            f << s.getFlatGrid() << std::endl;
        }
    }

    return ok;
}

