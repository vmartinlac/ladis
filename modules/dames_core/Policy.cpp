#include "Policy.h"

void Policy::enumerateChoices(const Checkers::State& state_from, ActionStateList& list)
{
    Checkers::ActionIterator it;
    it.init(state_from);

    list.clear();

    Checkers::Action action_;
    Checkers::State state_;
    while(it.next(state_from, action_, state_))
    {
        list.emplace_back(action_, state_);
    }
}

