
#pragma once

#include <tuple>
#include <vector>
#include <chrono>

using Clock = std::chrono::system_clock;

class Solver
{
public:

    using Operation = std::tuple<char,int,int,int>;

public:

    void solve(
        const std::vector<int>& numbers,
        int target,
        std::chrono::milliseconds time_budget,
        int num_solutions,
        std::vector< std::vector<Operation> >& solutions);
};

