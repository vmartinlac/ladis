#include <tuple>
#include "UniformPolicy.h"

UniformPolicy::UniformPolicy(int seed)
{
    myEngine.seed(seed);
}

bool UniformPolicy::getAction(const Checkers::State& state_from, Checkers::Action& action, Checkers::State& state_to)
{
    bool ret = false;

    Checkers::ActionIterator it;
    it.init(state_from);

    ActionStateList choices;
    enumerateChoices(state_from, choices);

    if(!choices.empty())
    {
        ret = true;

        const int index = myEngine() % choices.size();

        action = std::get<0>(choices[index]);
        state_to = std::get<1>(choices[index]);
    }

    return ret;
}

