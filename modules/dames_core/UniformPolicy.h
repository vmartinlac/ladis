
#pragma once

#include <random>
#include "Policy.h"

class UniformPolicy : public Policy
{
public:

    UniformPolicy(int seed);

    bool getAction(
        const Checkers::State& state_from,
        Checkers::Action& action,
        Checkers::State& state_to) override;

protected:

    std::default_random_engine myEngine;
};

