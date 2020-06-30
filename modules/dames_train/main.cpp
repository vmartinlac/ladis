#include <iostream>
#include <typeinfo>
#include <torch/torch.h>
#include "ActionDataset.h"

class ActionNetwork : public torch::nn::Module
{
public:

    ActionNetwork()
    {
        myWeight = register_parameter("w", torch::randn({10, 10}));
    }

    torch::Tensor forward(const Checkers::State& state_from, const Checkers::State& state_to)
    {
        torch::Tensor tensor_from = makeInputFeatures(state_from);
        torch::Tensor tensor_to = makeInputFeatures(state_to);

        return forward(tensor_from, tensor_to);
    }

    torch::Tensor forward(torch::Tensor state_from, torch::Tensor state_to)
    {
        // TODO
        return torch::full({1}, 0.0);
    }

    torch::Tensor makeInputFeatures(const Checkers::State& s)
    {
        torch::Tensor ret = torch::zeros({Checkers::N+2, Checkers::N+2, 6});

        // TODO

        return ret;
    }

protected:

    torch::Tensor myWeight;
};


int main(int num_args, char** args)
{
    //const QString root_directory = "/home/victor/Dépotoire/aa";
    const QString root_directory = "/home/victor/developpement/ladis/build/instancexx";
    const int num_epochs = 1;

    ActionDataset dataset;
    dataset.load(root_directory);

    Checkers::State state_from;
    std::vector<Checkers::State> states_to;
    std::vector<float> probabilities;
    std::vector<torch::Tensor> outputs;

    ActionNetwork network;

    torch::optim::Adam optimizer(network.parameters(), torch::optim::AdamOptions(1.0e-2));

    int epoch = 0;
    for(int epoch=0; epoch<num_epochs; epoch++)
    {
        std::cout << "Beginning epoch " << epoch << " ..." << std::endl;

        for(size_t i=0; i<dataset.getNumExamples(); i++)
        {
            dataset.getExample(i, state_from, states_to, probabilities);

            torch::Tensor tensor_from = network.makeInputFeatures(state_from);

            optimizer.zero_grad();

            outputs.clear();

            for(size_t j=0; j<states_to.size(); j++)
            {
                torch::Tensor tensor_to = network.makeInputFeatures(states_to[j]);

                outputs.push_back( network.forward(tensor_from, tensor_to) );
            }

            // TODO: compute error and compute gradient.

            optimizer.step();
        }
    }

    return 0;
}

