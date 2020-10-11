#include "Trainer.h"

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
    // TODO
}

