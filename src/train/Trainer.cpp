#include <memory>
#include <torch/torch.h>
#include "Trainer.h"

struct Net : torch::nn::Module
{
    Net()
    {
    }

    torch::Tensor forward(torch::Tensor x)
    {
        return x;
    }
};

void Trainer::train(const TransitionList& transitions)
{
    constexpr int num_epochs = 10;

    Checkers::ActionIterator ait;
    Checkers::State starting_state;
    Checkers::State available_state;
    Checkers::Action available_action;
    Checkers::Grid available_grid;
    std::vector<Checkers::Grid> target_grids;
    std::vector<float> target_probabilities;

    std::shared_ptr<Net> net = std::make_shared<Net>();

    torch::optim::SGD optimizer(net->parameters(), 0.01);

    for(int epoch=0; epoch<num_epochs; epoch++)
    {
        for(const TransitionList::value_type& transition : transitions)
        {
            starting_state.setMyTurn(true);
            starting_state.setGrid(transition.first);

            ait.init(starting_state);

            target_grids.clear();
            target_probabilities.clear();

            while(ait.next(starting_state, available_action, available_state))
            {
                available_grid = available_state.getGrid();

                target_grids.push_back(available_grid);

                if(available_grid == transition.second)
                {
                    target_probabilities.push_back(1.0f);
                }
                else
                {
                    target_probabilities.push_back(0.0f);
                }
            }

            learnSample(transition.first, target_grids, target_probabilities);
        }
    }
}

void Trainer::learnSample(const Checkers::Grid& grid0, const std::vector<Checkers::Grid>& grid1, const std::vector<float>& probability1)
{
    const int N = grid1.size();

    if(probability1.size() != N)
    {
        throw std::runtime_error("internal error");
    }

    torch::Tensor x = torch::zeros({N, 12, Checkers::SIDE+2, Checkers::SIDE+2});
    torch::Tensor y = torch::zeros({N});
    auto xx = x.accessor<float,4>();
    auto yy = y.accessor<float,1>();

    constexpr int INDEX_VOID = 0;
    constexpr int INDEX_EMPTY = 1;
    constexpr int INDEX_AGENT_MAN = 2;
    constexpr int INDEX_AGENT_QUEEN = 3;
    constexpr int INDEX_OPPONENT_MAN = 4;
    constexpr int INDEX_OPPONENT_QUEEN = 5;

    for(int i=0; i<N; i++)
    {
        int j=0;

        for(const Checkers::Grid& grid : {grid0, grid1[i]})
        {
            for(int k=0; k<Checkers::SIDE+2; k++)
            {
                xx[i][j+INDEX_VOID][k][0] = 1.0f;
                xx[i][j+INDEX_VOID][k][Checkers::SIDE+1] = 1.0f;
                xx[i][j+INDEX_VOID][0][k] = 1.0f;
                xx[i][j+INDEX_VOID][Checkers::SIDE+1][k] = 1.0f;
            }

            for(int k=0; k<Checkers::N; k++)
            {
                const int kk = Checkers::N - 1 - k;
                const int u = 2*(kk % 5) + (1 - (kk/5)%2);
                const int uu = 2*(kk % 5) + (kk/5)%2;
                const int v = kk / 5;

                switch(grid[k])
                {
                case 'p':
                    xx[i][j+INDEX_AGENT_MAN][u][v] = 1.0f;
                    break;
                case 'P':
                    xx[i][j+INDEX_AGENT_QUEEN][u][v] = 1.0f;
                    break;
                case 'o':
                    xx[i][j+INDEX_OPPONENT_MAN][u][v] = 1.0f;
                    break;
                case 'O':
                    xx[i][j+INDEX_OPPONENT_QUEEN][u][v] = 1.0f;
                    break;
                case '.':
                    xx[i][j+INDEX_EMPTY][u][v] = 1.0f;
                    break;
                default:
                    throw std::runtime_error("internal error!");
                }

                xx[i][j+INDEX_VOID][u][v] = 1.0f;
            }

            j += 6;
        }

        yy[i] = probability1[i];
    }
}

