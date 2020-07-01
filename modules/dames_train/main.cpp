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
        const int out_features_1 = 100;
        myWeights1 = register_parameter("w1", torch::randn({out_features_1, 50*5*2}));
        myBiais1 = register_parameter("b1", torch::randn({out_features_1}));

        const int out_features_2 = 1;
        myWeights2 = register_parameter("w2", torch::randn({out_features_2, out_features_1}));
        myBiais2 = register_parameter("b2", torch::randn({out_features_2}));
    }

    torch::Tensor forward(const Checkers::State& state_from, const std::vector<Checkers::State>& states_to)
    {
        myTemp0.clear();
        myTemp1.clear();

        myTemp0.resize(2);
        myTemp1.resize(states_to.size());

        myTemp0[0] = makeInputFeatures(state_from);

        for(size_t i=0; i<states_to.size(); i++)
        {
            myTemp0[1] = makeInputFeatures(states_to[i]);
            myTemp1[i] = torch::unsqueeze(torch::cat(myTemp0, 0), 0);
        }

        torch::Tensor L0 = torch::cat(myTemp1, 0); // (num_states, 500)
        torch::Tensor L1 = torch::nn::functional::linear(L0, myWeights1, myBiais1); // (num_states, out_features_1)
        torch::Tensor L2 = torch::nn::functional::relu(L1); // (num_states, out_features_1)
        torch::Tensor L3 = torch::nn::functional::linear(L2, myWeights2, myBiais2); // (num_states, out_features_2) == (num_states, 1)
        torch::Tensor L4 = torch::nn::functional::log_softmax(L3, 0); // (num_states, 1)

        myTemp0.clear();
        myTemp1.clear();

        return L4;
    }

    torch::Tensor makeTransitionProbabilityTable(const std::vector<float>& probabilities)
    {
        const size_t C = probabilities.size();
        torch::Tensor ret = torch::zeros({C});
        auto a = ret.accessor<float,1>();
        for(size_t i=0; i<C; i++)
        {
            a[i] = probabilities[i];
        }

        return ret;
    }

    torch::Tensor makeInputFeatures(const Checkers::State& s)
    {
        torch::Tensor ret = torch::zeros({5*Checkers::N});

        auto a = ret.accessor<float,1>();
        for(int i=0; i<Checkers::N; i++)
        {
            switch(s.readCell(i))
            {
            case 'o':
                a[5*i+0] = 1.0;
                break;
            case 'O':
                a[5*i+1] = 1.0;
                break;
            case 'p':
                a[5*i+2] = 1.0;
                break;
            case 'P':
                a[5*i+3] = 1.0;
                break;
            case '.':
                a[5*i+4] = 1.0;
                break;
            default:
                throw std::runtime_error("internal error");
            }
        }

        return ret;
    }

protected:

    std::vector<torch::Tensor> myTemp0;
    std::vector<torch::Tensor> myTemp1;

    torch::Tensor myWeights1;
    torch::Tensor myBiais1;
    torch::Tensor myWeights2;
    torch::Tensor myBiais2;
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
    const int num_epochs = 100;
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

    torch::optim::Adam optimizer(network.parameters(), torch::optim::AdamOptions(1.0e-4));

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
                torch::unsqueeze(network.forward(state_from, states_to), 0),
                torch::unsqueeze(network.makeTransitionProbabilityTable(probabilities), 0),
                torch::nn::KLDivLossOptions(torch::kBatchMean));

            loss.backward();

            optimizer.step();

            average_training_loss += loss.item<float>();
        }

        for(size_t i : testing)
        {
            dataset.getExample(i, state_from, states_to, probabilities);

            torch::Tensor loss = torch::nn::functional::kl_div(
                torch::unsqueeze(network.forward(state_from, states_to), 0),
                torch::unsqueeze(network.makeTransitionProbabilityTable(probabilities), 0),
                torch::nn::KLDivLossOptions(torch::kBatchMean));

            average_testing_loss += loss.item<float>();
        }

        average_training_loss /= double(training.size());
        average_testing_loss /= double(testing.size());

        std::cout << epoch << ' ' << average_training_loss << ' ' << average_testing_loss << std::endl;
    }

    return 0;
}

