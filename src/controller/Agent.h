
#pragma once

#include <vector>
#include <string>

class Agent
{
public:

    enum Result
    {
        RESULT_WON,
        RESULT_LOST,
        RESULT_MATCH,
        RESULT_ERROR
    };

public:

    virtual void beginMatch() = 0;
    virtual void play(const std::string& checkerboard, std::vector<int>& move) = 0;
    virtual void endMatch(int result) = 0;
};

