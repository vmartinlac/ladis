#include <random>
#include <iostream>
#include <typeinfo>
#include "ActionDataset.h"
#include "DatasetSubset.h"

class ActionNetwork : public torch::nn::Module
{
public:

    ActionNetwork()
    {
        const int out_features_1 = 100;
        myWeights1 = register_parameter("w1", torch::randn({out_features_1, 50*5*2}));
        myBiais1 = register_parameter("b1", torch::randn({out_features_1}));

        const int out_features_2 = 1;
        myWeights2 = register_parameter("w2", torch::randn({out_features_2, out_features_1}));
        myBiais2 = register_parameter("b2", torch::randn({out_features_2}));
    }

    torch::Tensor forward(torch::Tensor input)
    {
        torch::Tensor L1 = torch::nn::functional::linear(input, myWeights1, myBiais1); // (num_states, out_features_1)
        torch::Tensor L2 = torch::nn::functional::relu(L1); // (num_states, out_features_1)
        torch::Tensor L3 = torch::nn::functional::linear(L2, myWeights2, myBiais2); // (num_states, out_features_2) == (num_states, 1)
        torch::Tensor L4 = torch::nn::functional::log_softmax(L3, 0); // (num_states, 1)

        return L4;
    }

protected:

    torch::Tensor myWeights1;
    torch::Tensor myBiais1;
    torch::Tensor myWeights2;
    torch::Tensor myBiais2;
};

int main(int num_args, char** args)
{
    const QString root_directory = "/DATA/victor/developpement/ladis/data/";
    const int num_epochs = 1;
    const double seed = 101;
    const double training_samples_ratio = 0.9;

    std::default_random_engine RNG;
    RNG.seed(seed);

    ActionDataset whole_dataset;
    whole_dataset.load(root_directory);

    DatasetSubset training_dataset;
    DatasetSubset testing_dataset;

    DatasetSubset::split_into_test_train(
        whole_dataset,
        training_samples_ratio,
        RNG,
        training_dataset,
        testing_dataset);

    auto training_dataloader = torch::data::make_data_loader(training_dataset);
    auto testing_dataloader = torch::data::make_data_loader(testing_dataset);

    ActionNetwork network;

    torch::optim::Adam optimizer(network.parameters(), torch::optim::AdamOptions(1.0e-4));

    for(int epoch=0; epoch<num_epochs; epoch++)
    {
        std::cout << "Beginning epoch " << epoch << " ..." << std::endl;

        double average_training_loss = 0.0;
        double average_testing_loss = 0.0;

        for(const std::vector<ActionDataset::ExampleType>& examples : *training_dataloader)
        {
            optimizer.zero_grad();

            /*
            torch::Tensor loss = torch::nn::functional::kl_div(
                torch::unsqueeze(network.forward(state_from, states_to), 0),
                torch::unsqueeze(network.makeTransitionProbabilityTable(probabilities), 0),
                torch::nn::KLDivLossOptions(torch::kBatchMean));

            loss.backward();

            optimizer.step();

            average_training_loss += loss.item<float>();
            */
        }

        for(const std::vector<ActionDataset::ExampleType>& examples : *testing_dataloader)
        {
            /*
            torch::Tensor loss = torch::nn::functional::kl_div(
                torch::unsqueeze(network.forward(state_from, states_to), 0),
                torch::unsqueeze(network.makeTransitionProbabilityTable(probabilities), 0),
                torch::nn::KLDivLossOptions(torch::kBatchMean));

            average_testing_loss += loss.item<float>();
            */
        }

        average_training_loss /= double(*training_dataset.size());
        average_testing_loss /= double(*testing_dataset.size());

        std::cout << epoch << ' ' << average_training_loss << ' ' << average_testing_loss << std::endl;
    }

    return 0;
}

