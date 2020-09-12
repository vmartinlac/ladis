
#pragma once

#include <random>
#include "Agent.h"

class ModelBasedAgent : public Agent
{
public:

    ModelBasedAgent();

    void beforeEpisode() override;
    void afterEpisose() override;
    bool getAction(const Checkers::State& s, Checkers::Action& a) override;
};

