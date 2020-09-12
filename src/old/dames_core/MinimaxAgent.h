
#pragma once

#include "Agent.h"

class MinimaxAgent : public Agent
{
public:

    MinimaxAgent();

    void setMaxDepth(int max_depth);

    void beforeEpisode() override;
    void afterEpisose() override;
    bool getAction(const Checkers::State& s, Checkers::Action& a) override;

protected:

    int myMaxDepth;
    Checkers::Solver mySolver;
    Checkers::UtilityFunction myUtility;
};

