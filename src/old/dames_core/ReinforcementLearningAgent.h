
#pragma once

#include "Agent.h"

class ReinforcementLearningAgent : public Agent
{
public:

    ReinforcementLearningAgent();

    void beforeEpisode() override;
    void afterEpisose() override;
    bool getAction(const Checkers::State& s, Checkers::Action& a) override;
};

