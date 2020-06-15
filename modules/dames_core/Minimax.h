#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

/*
float State::getValue() const;
bool State::isMyTurn() const;
void State::makeDebugNodeSpec(std::ostream& s, const std::string& node_name, float value) const;

void Action::makeDebugEdgeSpec(std::ostream& s, const std::string& node0, const std::string& node1) const;

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

        std::string makeDebugNodeName() const
        {
            return std::string("n") + std::to_string(nodeid);
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

    Minimax()
    {
        myDebug = false;
    }

    void setDebug(bool x)
    {
        myDebug = x;
    }

    bool solve(const State& initial_state, Action& action, State& resulting_state, UtilityFunction& utility, int max_depth)
    {
        bool ret = false;

        if( initial_state.isMyTurn() )
        {
            std::vector<Node> nodes;
            nodes.reserve(max_depth);

            nodes.emplace_back();
            nodes.back().state = initial_state;
            nodes.back().nodeid = 0;

            float previous_value = 0.0f;
            Action candidate_action;
            State candidate_state;

            //std::stringstream debug_stream;
            std::ofstream debug_stream;
            if(myDebug)
            {
                debug_stream.open("graph.dot");
                debug_stream << "digraph {" << std::endl;
            }

            int node_count = 1;

            while(nodes.empty() == false)
            {
                if(nodes.size() >= max_depth)
                {
                    previous_value = utility.getValue( nodes.back().state );

                    if(myDebug)
                    {
                        nodes.back().state.makeDebugNodeSpec(debug_stream, nodes.back().makeDebugNodeName(), previous_value);
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

                        if(myDebug)
                        {
                            nodes.back().state.makeDebugNodeSpec(debug_stream, nodes.back().makeDebugNodeName(), previous_value);
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

                        if(myDebug)
                        {
                            next_action.makeDebugEdgeSpec( debug_stream, nodes[nodes.size()-2].makeDebugNodeName(), nodes.back().makeDebugNodeName() );
                        }
                    }
                    else if(nodes.back().has_best_value)
                    {
                        previous_value = nodes.back().best_value;

                        if(myDebug)
                        {
                            nodes.back().state.makeDebugNodeSpec(debug_stream, nodes.back().makeDebugNodeName(), previous_value);
                        }

                        nodes.pop_back();
                    }
                    else
                    {
                        previous_value = utility.getValue( nodes.back().state );

                        if(myDebug)
                        {
                            nodes.back().state.makeDebugNodeSpec(debug_stream, nodes.back().makeDebugNodeName(), previous_value);
                        }

                        nodes.pop_back();
                    }
                }
            }

            if(myDebug)
            {
                debug_stream << "}" << std::endl;
            }
        }

        return ret;
    }

protected:

    bool myDebug;
};

