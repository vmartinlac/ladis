
#pragma once

#include <torch/torch.h>
#include "ActionDataset.h"

class DatasetSubset : public torch::data::datasets::Dataset<DatasetSubset>
{
public:

    DatasetSubset(
        ActionDataset parent,
        std::vector<size_t>&& indices);

    DatasetSubset() = default;

    static void split_into_test_train(
        ActionDataset dataset,
        double train_ratio,
        unsigned seed,
        DatasetSubset& training_dataset,
        DatasetSubset& test_dataset);

    template<typename T>
    static void split_into_test_train(
        ActionDataset dataset,
        double train_ratio,
        T& rng,
        DatasetSubset& training_dataset,
        DatasetSubset& test_dataset);

    ExampleType get(size_t index) override;

    torch::optional<size_t> size() const override;

protected:

    std::vector<size_t> myIndices;
    ActionDataset myParent;
};

template<typename T>
void DatasetSubset::split_into_test_train(
    ActionDataset dataset,
    double training_ratio,
    T& rng,
    DatasetSubset& training_dataset,
    DatasetSubset& testing_dataset)
{
    const size_t num_samples = *dataset.size();

    std::vector<size_t> training;
    std::vector<size_t> testing;

    training.resize(num_samples);

    for(size_t i=0; i<num_samples; i++)
    {
        training[i] = i;
    }

    for(size_t i=0; i<num_samples; i++)
    {
        const size_t j = i + rng() % (num_samples-i);
        std::swap( training[i], training[j] );
    }

    while( double(training.size())/double(num_samples) > training_ratio )
    {
        testing.push_back(training.back());
        training.pop_back();
    }

    training_dataset = DatasetSubset(dataset, std::move(training));
    testing_dataset = DatasetSubset(dataset, std::move(testing));
}

