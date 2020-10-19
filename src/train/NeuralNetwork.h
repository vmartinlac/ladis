
#pragma once

#include <torch/torch.h>
#include "Checkers.h"

struct NeuralNetwork : torch::nn::Module
{
    NeuralNetwork();

    torch::Tensor forward(
        const Checkers::Grid& grid_from,
        std::vector<Checkers::Grid>& grids_to);

    torch::nn::Conv2d myConv;
    torch::nn::Linear myLinear0;
    torch::nn::Linear myLinear1;

private:

    Checkers::ActionIterator myActionIterator;
};

using NeuralNetworkHolder = torch::nn::ModuleHolder<NeuralNetwork>;

