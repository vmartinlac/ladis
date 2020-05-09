#include <sstream>
#include <stack>
#include <memory>
#include <iostream>
#include "Solver.h"

void Solver::solve(const std::vector<int>& numbers, int target, std::chrono::milliseconds time_budget, int num_solutions, std::vector< std::vector<Operation> >& _solutions)
{
    struct Node
    {
        std::vector<int> stock;
        std::vector<Operation> operations;
        int distance_to_target;

        void update_distance_to_target(int target)
        {
            distance_to_target = std::abs(stock.front() - target);

            for(int value : stock)
            {
                distance_to_target = std::min( distance_to_target, std::abs(value - target) );
            }
        }

        std::string to_string()
        {
            std::stringstream ss;

            for(Operation op : operations)
            {
                ss << std::get<1>(op) << " " << std::get<0>(op) << " " << std::get<2>(op) << " = " << std::get<3>(op) << std::endl;
            }

            return ss.str();
        }
    };

    using NodePtr = std::shared_ptr<Node>;

    const char* operations = "+-*/";

    _solutions.clear();

    if(numbers.empty() == false)
    {
        // compute initial node.

        NodePtr first_node(new Node());

        first_node->stock.assign(numbers.begin(), numbers.end());
        first_node->update_distance_to_target(target);

        std::vector<NodePtr> solutions{first_node};

        std::stack<NodePtr> stack;
        stack.push(first_node);

        const Clock::time_point t0 = Clock::now();

        while(
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t0) < time_budget &&
            (solutions.front()->distance_to_target > 0 || solutions.size() < num_solutions) &&
            stack.empty() == false)
        {
            // pop one node.

            NodePtr this_node = stack.top();
            stack.pop();

            // check whether this node is better than the one we deemed best.

            if(solutions.front()->distance_to_target == 0)
            {
                if(this_node->distance_to_target == 0)
                {
                    solutions.push_back(this_node);
                }
            }
            else
            {
                if(this_node->distance_to_target == 0)
                {
                    solutions.clear();
                    solutions.push_back(this_node);
                }
                else if( this_node->distance_to_target < solutions.front()->distance_to_target )
                {
                    solutions.front() = this_node;
                }
            }

            for(int i=0; i<this_node->stock.size(); i++)
            {
                for(int j=0; j<this_node->stock.size(); j++)
                {
                    if(i != j)
                    {
                        for(char op : "+-*/")
                        {
                            const int a = this_node->stock[i];
                            const int b = this_node->stock[j];
                            int c = 0;
                            bool authorized = false;

                            if(op == '+')
                            {
                                c = a+b;
                                authorized = (i<j);
                            }
                            else if(op == '-')
                            {
                                c = a-b;
                                authorized = (c >= 0);
                                //authorized = true;
                            }
                            else if(op == '*')
                            {
                                c = a*b;
                                authorized = (i<j);
                            }
                            else if(op == '/')
                            {
                                authorized = (b != 0 && a % b == 0);

                                if(authorized)
                                {
                                    c = a/b;
                                }
                            }
                            else
                            {
                                authorized = false;
                            }

                            if(authorized)
                            {
                                NodePtr new_node(new Node());

                                for(int k=0; k<this_node->stock.size(); k++)
                                {
                                    if(k != i && k != j)
                                    {
                                        new_node->stock.push_back(this_node->stock[k]);
                                    }
                                }

                                new_node->stock.push_back(c);

                                new_node->update_distance_to_target(target);

                                new_node->operations.assign(this_node->operations.begin(), this_node->operations.end());
                                new_node->operations.emplace_back(op, a, b, c);

                                stack.push(new_node);
                            }
                        }
                    }
                }
            }
        }

        for(NodePtr s : solutions)
        {
            std::cout << "SOLUTION" << std::endl;
            std::cout << s->to_string() << std::endl;

            _solutions.push_back(std::move(s->operations));
        }
    }
}

