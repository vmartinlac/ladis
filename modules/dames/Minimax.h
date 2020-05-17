#pragma once

#include <vector>
#include <stdexcept>

/*
float State::getValue();
bool State::isMyTurn();

void ActionIterator::init(const State& s);
bool ActionIterator::next(const State& state_from, Action& action, State& state_to);
*/

template<typename State, typename Action, typename ActionIterator>
class Minimax
{
protected:

    struct Node
    {
        Node()
        {
            is_investigating_action = false;
            has_best_value = false;
            best_value = 0.0f;
        }

        State state;
        ActionIterator action_iterator;

        bool is_investigating_action;
        bool has_best_value;
        float best_value;

        // TODO: alpha-beta pruning
    };

public:

    Minimax() = default;

    bool solve(const State& initial_state, Action& action, State& resulting_state, int max_depth)
    {
        bool ret = false;

        if( initial_state.isMyTurn() )
        {
            std::vector<Node> nodes;
            nodes.reserve(max_depth);

            nodes.emplace_back();
            nodes.back().state = initial_state;

            float previous_value = 0.0f;
            Action candidate_action;
            State candidate_state;

            while(nodes.empty() == false)
            {
                if(nodes.size() >= max_depth)
                {
                    previous_value = nodes.back().state.getValue();

                    nodes.pop_back();
                }
                else
                {
                    if(nodes.back().is_investigating_action)
                    {
                        const bool take_it =
                            (nodes.back().has_best_value == false) ||
                            (nodes.back().state.isMyTurn() == true && nodes.back().best_value < previous_value) ||
                            (nodes.back().state.isMyTurn() == false && nodes.back().best_value > previous_value);

                        if(take_it)
                        {
                            nodes.back().has_best_value = true;
                            nodes.back().best_value = previous_value;

                            if(nodes.size() == 1)
                            {
                                action = candidate_action;
                                resulting_state = candidate_state;
                                ret = true;
                            }

                            // TODO: update alpha and beta.
                        }
                    }
                    else
                    {
                        nodes.back().action_iterator.init(nodes.back().state);
                    }

                    Action next_action;
                    State next_state;

                    if( nodes.back().action_iterator.next(nodes.back().state, next_action, next_state) )
                    {
                        if(nodes.size() == 1)
                        {
                            candidate_action = next_action;
                            candidate_state = next_state;
                        }

                        nodes.back().is_investigating_action = true;

                        nodes.emplace_back();
                        nodes.back().state = next_state;
                    }
                    else if(nodes.back().has_best_value)
                    {
                        previous_value = nodes.back().best_value;
                        nodes.pop_back();
                    }
                    else
                    {
                        previous_value = nodes.back().state.getValue();
                        nodes.pop_back();
                    }
                }
            }
        }

        return ret;
    }
};

