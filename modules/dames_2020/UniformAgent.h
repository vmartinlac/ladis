
#pragma once

#include <random>
#include "Agent.h"

class UniformAgent : public Agent
{
public:

    UniformAgent();

    void seed(int s);

    void beforeEpisode() override;
    void afterEpisose() override;
    bool getAction(const Checkers::State& s, Checkers::Action& a) override;

protected:

    std::default_random_engine myEngine;
    std::vector<Checkers::Action> myActions;
};

