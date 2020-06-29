#include <iostream>
#include <typeinfo>
#include <torch/torch.h>
#include "ActionDataset.h"

struct ActionNetwork : torch::nn::Module
{
    ActionNetwork()
    {
        w = register_parameter("w", torch::randn({10, 10}));
    }

    torch::Tensor forward(torch::Tensor input)
    {
        // TODO
        return torch::Tensor();
    }

    torch::Tensor w;
};

torch::Tensor encodeCheckersState(const Checkers::State& s)
{
    // TODO
}

int main(int num_args, char** args)
{
    const QString root_directory = "/home/victor/Dépotoire/aa";

    ActionDataset dataset;
    dataset.load(root_directory);

    Checkers::State state_from;
    std::vector<Checkers::State> state_to;
    std::vector<float> transition_probability;

    ActionNetwork network;

    torch::optim::Adam optimizer(network.parameters());

    int epoch = 0;
    while(true)
    {
        for(size_t i=0; i<dataset.getNumExamples(); i++)
        {
            dataset.getExample(i, state_from, state_to, transition_probability);

            optimizer.zero_grad();

            optimizer.step();
        }

        epoch++;
        break;
    }

    return 0;
}

