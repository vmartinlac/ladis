
#pragma once

#include "Checkers.h"

class Model
{
public:

    using WeightedAction = std::tuple<double, Checkers::Action>;
    using Sample = std::tuple<Checkers::State, Checkers::Action>;

    Model();

    void learn(const std::vector<Sample>& samples);
    void predict(const Checkers::State& state, std::vector<WeightedAction>& weighted_actions);
    bool save(const std::string& path);
    bool load(const std::string& path);
};

