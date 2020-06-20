#include "Model.h"

Model::Model()
{
}

void Model::learn(const std::vector<Sample>& samples)
{
    throw std::runtime_error("not implemented");
}

void Model::predict(const Checkers::State& state, std::vector<WeightedAction>& weighted_actions)
{
    throw std::runtime_error("not implemented");
}

bool Model::save(const std::string& path)
{
    return false;
}

bool Model::load(const std::string& path)
{
    return false;
}

