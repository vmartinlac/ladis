#include <iostream>
#include <sstream>
#include <unistd.h>
#include "SaverAgent.h"
#include "Utils.h"

SaverAgent::SaverAgent(Agent* backend)
{
    myBackend = backend;
    myInterrupted = false;
}

std::string SaverAgent::getName()
{
    return myBackend->getName();
}

MatchLogPtr SaverAgent::retrieveMatchLog()
{
    return std::move(myCurrentMatch);
}

void SaverAgent::beginMatch(bool agent_plays_first, int difficulty)
{
    myBackend->beginMatch(agent_plays_first, difficulty);

    myCurrentMatch.reset(new MatchLog());
    myInterrupted = false;

    myCurrentMatch->agent_plays_first = agent_plays_first;
    myCurrentMatch->difficulty = difficulty;
    myCurrentMatch->agent_moves.clear();
    myCurrentMatch->agent = myBackend->getName();
    myCurrentMatch->start_timestamp = ClockType::now();

    {
        char buffer[512];

        if(gethostname(buffer, 512) != 0)
        {
            std::cerr << "gethostname() failed!" << std::endl;
            exit(1);
        }

        myCurrentMatch->hostname = buffer;
    }

    myStartTime = ClockType::now();
}

bool SaverAgent::play(const Checkers::State& state, Checkers::Action& action)
{
    ClockType::time_point t1 = ClockType::now();

    const bool ret = myBackend->play(state, action);

    ClockType::time_point t2 = ClockType::now();

    if(myCurrentMatch)
    {
        if(ret)
        {
            if(myInterrupted)
            {
                myCurrentMatch.reset();
                std::cerr << "Very strange: game went on despite the agent having failed to deliver an action at some point." << std::endl;
            }
            else
            {
                myCurrentMatch->agent_moves.emplace_back();

                myCurrentMatch->agent_moves.back().timestamp_before_computation = t1;
                myCurrentMatch->agent_moves.back().timestamp_after_computation = t2;
                myCurrentMatch->agent_moves.back().state = state;
                myCurrentMatch->agent_moves.back().action = action;
            }
        }
        else
        {
            myInterrupted = true;
        }
    }

    return ret;
}

void SaverAgent::endMatch(int result)
{
    myBackend->endMatch(result);

    if(myCurrentMatch)
    {
        myCurrentMatch->result = result;
    }
}

