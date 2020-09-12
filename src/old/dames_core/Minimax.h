#pragma once

#include <vector>
#include <stdexcept>

/*
float State::getValue() const;
bool State::isMyTurn() const;

float UtilityFunction::getValue(const State& s) const;

void ActionIterator::init(const State& s);
bool ActionIterator::next(const State& state_from, Action& action, State& state_to);
*/

template<typename State, typename UtilityFunction, typename Action, typename ActionIterator>
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
            has_min_pruning_value = false;
            has_max_pruning_value = false;
            min_pruning_value = 0.0f;
            max_pruning_value = 0.0f;
            nodeid = -1;
        }

        State state;
        ActionIterator action_iterator;

        int nodeid;

        bool is_investigating_action;
        bool has_best_value;
        float best_value;

        bool has_min_pruning_value;
        bool has_max_pruning_value;
        float min_pruning_value;
        float max_pruning_value;
    };

public:

    class Hook
    {
    public:

        virtual void hookStarted()
        {
        }

        virtual void hookFinished()
        {
        }

        virtual void hookCompletedNode(const State& s, int nodeid, float value)
        {
        }

        virtual void hookFoundEdge(const Action& action, int nodeid_from, int nodeid_to)
        {
        }
    };

    Minimax()
    {
        myHook = nullptr;
    }

    void resetHook(Hook* hook=nullptr)
    {
        myHook = hook;
    }

    bool solve(const State& initial_state, Action& action, State& resulting_state, UtilityFunction& utility, int max_depth)
    {
        bool ret = false;

        if( initial_state.isMyTurn() )
        {
            if(myHook)
            {
                myHook->hookStarted();
            }

            std::vector<Node> nodes;
            nodes.reserve(max_depth);

            nodes.emplace_back();
            nodes.back().state = initial_state;
            nodes.back().nodeid = 0;

            float previous_value = 0.0f;
            Action candidate_action;
            State candidate_state;

            int node_count = 1;

            while(nodes.empty() == false)
            {
                if(nodes.size() >= max_depth)
                {
                    previous_value = utility.getValue( nodes.back().state );

                    if(myHook)
                    {
                        myHook->hookCompletedNode(
                            nodes.back().state,
                            nodes.back().nodeid,
                            previous_value);
                    }

                    nodes.pop_back();
                }
                else
                {
                    bool prune = false;

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

                            if(nodes.back().state.isMyTurn())
                            {
                                prune = nodes.back().has_min_pruning_value && nodes.back().min_pruning_value < nodes.back().best_value;

                                if( nodes.back().has_max_pruning_value )
                                {
                                    nodes.back().max_pruning_value = std::max(nodes.back().max_pruning_value, nodes.back().best_value);
                                }
                                else
                                {
                                    nodes.back().has_max_pruning_value = true;
                                    nodes.back().max_pruning_value = nodes.back().best_value;
                                }
                            }
                            else
                            {
                                prune = nodes.back().has_max_pruning_value && nodes.back().max_pruning_value > nodes.back().best_value;

                                if( nodes.back().has_min_pruning_value )
                                {
                                    nodes.back().min_pruning_value = std::min(nodes.back().min_pruning_value, nodes.back().best_value);
                                }
                                else
                                {
                                    nodes.back().has_min_pruning_value = true;
                                    nodes.back().min_pruning_value = nodes.back().best_value;
                                }
                            }
                        }
                    }
                    else
                    {
                        nodes.back().action_iterator.init(nodes.back().state);
                    }

                    Action next_action;
                    State next_state;

                    if(prune)
                    {
                        previous_value = nodes.back().best_value;

                        if(myHook)
                        {
                            myHook->hookCompletedNode(
                                nodes.back().state,
                                nodes.back().nodeid,
                                previous_value);
                        }

                        nodes.pop_back();
                    }
                    else if( nodes.back().action_iterator.next(nodes.back().state, next_action, next_state) )
                    {
                        if(nodes.size() == 1)
                        {
                            candidate_action = next_action;
                            candidate_state = next_state;
                        }

                        nodes.back().is_investigating_action = true;

                        nodes.emplace_back();
                        nodes.back().state = next_state;
                        nodes.back().has_min_pruning_value = nodes[nodes.size()-2].has_min_pruning_value;
                        nodes.back().has_max_pruning_value = nodes[nodes.size()-2].has_max_pruning_value;
                        nodes.back().min_pruning_value = nodes[nodes.size()-2].min_pruning_value;
                        nodes.back().max_pruning_value = nodes[nodes.size()-2].max_pruning_value;
                        nodes.back().nodeid = node_count;
                        node_count++;

                        if(myHook)
                        {
                            myHook->hookFoundEdge(
                                next_action,
                                nodes[nodes.size()-2].nodeid,
                                nodes[nodes.size()-1].nodeid);
                        }
                    }
                    else if(nodes.back().has_best_value)
                    {
                        previous_value = nodes.back().best_value;

                        if(myHook)
                        {
                            myHook->hookCompletedNode(
                                nodes.back().state,
                                nodes.back().nodeid,
                                previous_value);
                        }

                        nodes.pop_back();
                    }
                    else
                    {
                        previous_value = utility.getValue( nodes.back().state );

                        if(myHook)
                        {
                            myHook->hookCompletedNode(
                                nodes.back().state,
                                nodes.back().nodeid,
                                previous_value);
                        }

                        nodes.pop_back();
                    }
                }
            }

            if(myHook)
            {
                myHook->hookFinished();
            }
        }

        return ret;
    }

protected:

    Hook* myHook;
};

