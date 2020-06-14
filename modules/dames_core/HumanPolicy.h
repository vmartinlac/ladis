
#pragma once

#include "Policy.h"

class HumanPolicy : public Policy
{
public:

    bool getAction(
        const Checkers::State& state_from,
        Checkers::Action& action,
        Checkers::State& state_to) override;
};

