#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork() :
    myConv(12, 10, 3),
    myLinear0(10*3*3, 5),
    myLinear1(5, 1)
{
    register_module("conv", myConv);
    register_module("linear0", myLinear0);
    register_module("linear1", myLinear1);
}

torch::Tensor NeuralNetwork::forward(const Checkers::Grid& grid_from, std::vector<Checkers::Grid>& grids_to)
{
    {
        Checkers::State state_from;
        Checkers::Action action;
        Checkers::State state_to;

        state_from.setGrid(grid_from);
        state_from.setMyTurn(false);

        myActionIterator.init(state_from);
        grids_to.clear();

        while(myActionIterator.next(state_from, action, state_to))
        {
            grids_to.push_back(state_to.getGrid());
        } 
    }

    const int N = grids_to.size();

    torch::Tensor x = torch::zeros({N, 12, Checkers::SIDE+2, Checkers::SIDE+2});

    {
        auto xx = x.accessor<float,4>();

        constexpr int INDEX_VOID = 0;
        constexpr int INDEX_EMPTY = 1;
        constexpr int INDEX_AGENT_MAN = 2;
        constexpr int INDEX_AGENT_QUEEN = 3;
        constexpr int INDEX_OPPONENT_MAN = 4;
        constexpr int INDEX_OPPONENT_QUEEN = 5;

        for(int i=0; i<N; i++)
        {
            int j=0;

            for(const Checkers::Grid& grid : {grid_from, grids_to[i]})
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
        }
    }

    x = x.to(torch::kCUDA);

    x = myConv(x);
    x = torch::nn::functional::max_pool2d(x, torch::nn::functional::MaxPool2dFuncOptions(3));
    x = torch::nn::functional::relu(x);
    x = torch::nn::functional::dropout2d(x, torch::nn::functional::DropoutFuncOptions().p(0.25));
    x = x.view({N, -1});
    x = myLinear0(x);
    x = torch::nn::functional::relu(x);
    x = myLinear1(x);

    return x;
}

