
#pragma once

#include "TransitionList.h"

class Trainer
{
public:

    void train(const TransitionList& transitions);

protected:

    void learnSample(
        const Checkers::Grid& grid0,
        const std::vector<Checkers::Grid>& grid1,
        const std::vector<float>& probability1);
};

