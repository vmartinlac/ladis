#include <LADIS.h>
#include "Sensor.h"

void Sensor::sense()
{
    // TODO
}

Sensor::FrameType Sensor::getFrameType()
{
    return myFrameType;
}

bool Sensor::getForbiddenMove()
{
    return myForbiddenMove;
}

void Sensor::getState(Checkers::State& state)
{
    state = myState;
}

int Sensor::getDifficulty()
{
    return myDifficulty;
}

bool Sensor::getPlayerStarts()
{
    return myPlayerStarts;
}

bool Sensor::getOpponentStarts()
{
    return myOpponentStarts;
}

bool Sensor::getGameMode()
{
    return myGameMode;
}

bool Sensor::getProblemMode()
{
    return myProblemMode;
}

const std::vector<int>& Sensor::gefTrajectoryPrompt()
{
    return myTrajectoryPrompt;
}

