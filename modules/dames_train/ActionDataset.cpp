#include "ActionDataset.h"

bool ActionDataset::load(const QString& path)
{
    myLogLoader.reset(new LogLoader());
    return myLogLoader->load(path);
}

ActionDataset::ExampleType ActionDataset::get(size_t index)
{
    constexpr int K = 5;

    Checkers::State state_from;
    Checkers::State state_to;
    float probability = 0.0f;

    myLogLoader->getExample(index, state_from, state_to, probability);

    torch::Tensor input = torch::zeros({2*Checkers::N*K});
    torch::Tensor output = torch::full({1}, probability);

    Checkers::State* states[2];
    states[0] = &state_from;
    states[1] = &state_to;

    auto accessor = input.accessor<float,1>();

    for(int i=0; i<2; i++)
    {
        const Checkers::State& state = *states[i];

        for(int j=0; j<Checkers::N; j++)
        {
            int k = 0;

            switch(state.readCell(j))
            {
            case '.':
                k = 0;
                break;
            case 'o':
                k = 1;
                break;
            case 'O':
                k = 2;
                break;
            case 'p':
                k = 3;
                break;
            case 'P':
                k = 4;
                break;
            default:
                throw std::runtime_error("Internal error");
            }

            accessor[i*K*Checkers::N + K*j + k] = 1.0;
        }
    }

    return ExampleType(input, output);
}

torch::optional<size_t> ActionDataset::size() const
{
    return myLogLoader->getNumExamples();
}

