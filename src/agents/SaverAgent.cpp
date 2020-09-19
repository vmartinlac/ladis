#include <iostream>
#include "SaverAgent.h"
#include "Utils.h"

SaverAgent::SaverAgent(Database* db, Agent* backend)
{
    myDatabase = db;
    myBackend = backend;
    myHasCurrentMatch = false;
    myMoveCount = 0;
    myInterrupted = false;
}

std::string SaverAgent::getName()
{
    return myBackend->getName();
}

void SaverAgent::beginMatch(bool agent_plays_first, int difficulty)
{
    myBackend->beginMatch(agent_plays_first, difficulty);

    myHasCurrentMatch = true;
    myInterrupted = false;

    myCurrentMatch.agent_plays_first = agent_plays_first;
    myCurrentMatch.difficulty = difficulty;
    myCurrentMatch.agent_moves.clear();
    myCurrentMatch.agent = myBackend->getName();
    myCurrentMatch.start_datetime = Utils::getCurrentDateTime();

    myStartTime = ClockType::now();
}

bool SaverAgent::play(const Checkers::State& state, Checkers::Action& action)
{
    ClockType::time_point t1 = ClockType::now();

    const bool ret = myBackend->play(state, action);

    ClockType::time_point t2 = ClockType::now();

    if(ret)
    {
        if(myInterrupted)
        {
            myHasCurrentMatch = false;
            std::cerr << "Very strange: game went on despite the agent having failed to deliver an action at some point." << std::endl;
        }
        else
        {
            myCurrentMatch.agent_moves.emplace_back();

            myCurrentMatch.agent_moves.back().computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() * 1.0e-3;
            myCurrentMatch.agent_moves.back().time_offset = std::chrono::duration_cast<std::chrono::milliseconds>(t2-myStartTime).count() * 1.0e-3;
            myCurrentMatch.agent_moves.back().state = state;
            myCurrentMatch.agent_moves.back().action = action;
        }
    }
    else
    {
        myInterrupted = true;
    }

    return ret;
}

void SaverAgent::endMatch(int result)
{
    myBackend->endMatch(result);

    if(myHasCurrentMatch)
    {
        myCurrentMatch.result = result;
        myDatabase->saveMatch(myCurrentMatch);
    }
}

