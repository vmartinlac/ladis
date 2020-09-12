
#pragma once

#include <QString>
#include <torch/torch.h>
#include "LogLoader.h"

class ActionDataset : public torch::data::datasets::Dataset<ActionDataset>
{
public:

    /*
    ActionDataset() = default;

    ActionDataset(const ActionDataset& o) = delete;

    ActionDataset& operator=(const ActionDataset& o) = delete;
    */

    bool load(const QString& path);

    ExampleType get(size_t index) override;

    torch::optional<size_t> size() const override;

protected:

    std::shared_ptr<LogLoader> myLogLoader;
};

