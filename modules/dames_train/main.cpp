#include <random>
#include <iostream>
#include <typeinfo>
#include <torch/torch.h>
#include "ActionDataset.h"

class ActionNetwork : public torch::nn::Module
{
public:

    ActionNetwork()
    {
        //myWeight = register_parameter("w", torch::randn({10, 10}));
    }

    torch::Tensor forward(const Checkers::State& state_from, const std::vector<Checkers::State>& states_to)
    {
        /*
        torch::Tensor tensor_from = makeInputFeatures(state_from);
        torch::Tensor tensor_to = makeInputFeatures(state_to);

        return forward(tensor_from, tensor_to);
        */

        const size_t C = states_to.size();

        torch::Tensor tmp = torch::full({1, C}, 0.0);
        return torch::nn::functional::log_softmax(tmp, 1);
    }

    torch::Tensor makeTransitionProbabilityTable(const std::vector<float>& probabilities)
    {
        const size_t C = probabilities.size();
        return torch::full({1, C}, 0.0);
    }

    /*
    torch::Tensor forward(torch::Tensor state_from, std::vector<torch::Tensor> states_to)
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
    */

protected:

    //torch::Tensor myWeight;
};

void makeTrainingTestingSets(size_t num_samples, unsigned seed, double training_ratio, std::vector<size_t>& training, std::vector<size_t>& testing)
{
    std::default_random_engine engine;
    engine.seed(seed);

    training.resize(num_samples);

    for(size_t i=0; i<num_samples; i++)
    {
        training[i] = i;
    }

    for(size_t i=0; i<num_samples; i++)
    {
        const size_t j = i + engine() % (num_samples-i);
        std::swap( training[i], training[j] );
    }

    while( double(training.size())/double(num_samples) > training_ratio )
    {
        testing.push_back(training.back());
        training.pop_back();
    }
}

int main(int num_args, char** args)
{
    //const QString root_directory = "/home/victor/Dépotoire/aa";
    const QString root_directory = "/home/victor/developpement/ladis/build/instancexx";
    const int num_epochs = 1;
    const double datasets_seed = 101;
    const double training_samples_ratio = 0.9;

    //

    ActionDataset dataset;
    dataset.load(root_directory);

    std::vector<size_t> training;
    std::vector<size_t> testing;
    makeTrainingTestingSets(dataset.getNumExamples(), datasets_seed, training_samples_ratio, training, testing);

    Checkers::State state_from;
    std::vector<Checkers::State> states_to;
    std::vector<float> probabilities;

    ActionNetwork network;

    torch::optim::Adam optimizer(network.parameters(), torch::optim::AdamOptions(1.0e-2));

    int epoch = 0;
    for(int epoch=0; epoch<num_epochs; epoch++)
    {
        std::cout << "Beginning epoch " << epoch << " ..." << std::endl;

        double average_training_loss = 0.0;
        double average_testing_loss = 0.0;

        for(size_t i : training)
        {
            dataset.getExample(i, state_from, states_to, probabilities);

            optimizer.zero_grad();

            torch::Tensor loss = torch::nn::functional::kl_div(
                network.forward(state_from, states_to),
                network.makeTransitionProbabilityTable(probabilities));

            loss.backward();

            optimizer.step();

            average_training_loss += loss.item<float>();
        }

        for(size_t i : testing)
        {
            dataset.getExample(i, state_from, states_to, probabilities);

            torch::Tensor loss = torch::nn::functional::kl_div(
                network.forward(state_from, states_to),
                network.makeTransitionProbabilityTable(probabilities));

            average_testing_loss += loss.item<float>();
        }

        average_training_loss /= double(training.size());
        average_testing_loss /= double(testing.size());
        std::cout << epoch << ' ' << average_training_loss << ' ' << average_testing_loss << std::endl;
    }

    return 0;
}

