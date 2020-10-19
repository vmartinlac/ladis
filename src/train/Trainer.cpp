#include <memory>
#include "NeuralNetwork.h"
#include "Trainer.h"

void Trainer::train(const TransitionList& transitions)
{
    constexpr int num_epochs = 100;

    std::vector<Checkers::Grid> grids_to;

    NeuralNetworkHolder net;
    net->to(torch::kCUDA);

    torch::optim::SGD optimizer(net->parameters(), 0.0005);

    for(int epoch=0; epoch<num_epochs; epoch++)
    {
        std::cout << "Beginning epoch " << epoch << std::endl;

        int num_good = 0;
        int total = 0;

        for(const TransitionList::value_type& transition : transitions)
        {
            optimizer.zero_grad();

            torch::Tensor probabilities = net->forward(transition.first, grids_to);

            std::vector<Checkers::Grid>::iterator it = std::find(grids_to.begin(), grids_to.end(), transition.second);

            if(it == grids_to.end())
            {
                throw std::runtime_error("internal error");
            }

            const int good_grid = std::distance(grids_to.begin(), it);
            torch::Tensor target = torch::full({1,1}, good_grid, torch::TensorOptions().dtype(torch::kLong)).to(torch::kCUDA);

            torch::Tensor err = torch::nn::functional::cross_entropy(torch::unsqueeze(probabilities, 0), target);
            err.backward();

            optimizer.step();

            //err = err.to(torch::kCPU);

            std::tuple<torch::Tensor, torch::Tensor> rm = torch::max(probabilities, 0);
            if( std::get<1>(rm).item<int32_t>() == good_grid )
            {
                num_good++;
            }
            total++;
        }

        std::cout << "Score: " << double(num_good)/double(total) << std::endl;
    }
}

