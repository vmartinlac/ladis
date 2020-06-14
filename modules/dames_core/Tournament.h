
#pragma once

#include "Policy.h"

class Tournament
{
public:

    Tournament();

    void setVerbose(bool verbose)
    {
        myVerbose = verbose;
    }

    void setYield(bool yield)
    {
        myYield = yield;
    }

    bool run(Policy* p1, Policy* p2, int& winner);

protected:

    bool myVerbose;
    bool myYield;
};

