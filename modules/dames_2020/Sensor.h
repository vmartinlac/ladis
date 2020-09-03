
#pragma once

#include <vector>
#include <Checkers.h>

class Sensor
{
public:

    enum FrameType
    {
        FRAME_MENU,
        FRAME_QUITTING,
        FRAME_WON,
        FRAME_LOST,
        FRAME_OPPONENT_PLAYS,
        FRAME_PLAYER_PLAYS
    };

public:

    void sense();

    FrameType getFrameType();
    bool getForbiddenMove();
    void getState(Checkers::State& state);
    int getDifficulty();
    bool getPlayerStarts();
    bool getOpponentStarts();
    bool getGameMode();
    bool getProblemMode();
    const std::vector<int>& gefTrajectoryPrompt();

protected:

    FrameType myFrameType;
    bool myForbiddenMove;
    Checkers::State myState;
    int myDifficulty;
    bool myPlayerStarts;
    bool myOpponentStarts;
    bool myGameMode;
    bool myProblemMode;
    std::vector<int> myTrajectoryPrompt;
};

