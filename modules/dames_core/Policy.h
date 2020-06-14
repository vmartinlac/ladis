
#pragma once

#include <tuple>
#include "Checkers.h"

class Policy
{
public:

    Policy() = default;

    void setName(const std::string& name)
    {
        myName = name;
    }

    std::string getName() const
    {
        return myName;
    }

    virtual bool getAction(
        const Checkers::State& state_from,
        Checkers::Action& action,
        Checkers::State& state_to) = 0;

protected:

    using ActionState = std::tuple<Checkers::Action, Checkers::State>;
    using ActionStateList = std::vector<ActionState>;

    static void enumerateChoices(
        const Checkers::State& state_from,
        ActionStateList& list);

protected:

    std::string myName;
};

