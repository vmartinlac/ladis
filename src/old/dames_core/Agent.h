
#pragma once

#include "Checkers.h"

class Agent
{
public:

    virtual void beforeEpisode() = 0;
    virtual void afterEpisose() = 0;
    virtual bool getAction(const Checkers::State& s, Checkers::Action& a) = 0;
};

