#include "DatasetSubset.h"

DatasetSubset::DatasetSubset(
    ActionDataset parent,
    std::vector<size_t>&& indices)
{
    myIndices = std::move(indices);
    myParent = std::move(parent);
}

DatasetSubset::ExampleType DatasetSubset::get(size_t index)
{
    return myParent.get(myIndices[index]);
}

torch::optional<size_t> DatasetSubset::size() const
{
    return myIndices.size();
}

void DatasetSubset::split_into_test_train(
    ActionDataset dataset,
    double train_ratio,
    unsigned seed,
    DatasetSubset& training_dataset,
    DatasetSubset& test_dataset)
{
    std::default_random_engine rng;
    rng.seed(seed);

    split_into_test_train(dataset, train_ratio, rng, training_dataset, test_dataset);
}

