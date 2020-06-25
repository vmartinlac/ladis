
#pragma once

#include <map>
#include <vector>
#include <torch/torch.h>
#include <Checkers.h>
#include <QString>

class ActionDataset : public torch::data::datasets::Dataset<ActionDataset>
{
protected:

    struct Example
    {
        Checkers::State state_from;
        Checkers::State state_to;
        float probability;
    };

public:

    bool load(const QString& path);
    ExampleType get(size_t index) override;
    c10::optional<size_t> size() const override;

protected:

    void parseRootDirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);
    void parseSubdirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);
    void parseFile(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);

protected:

    std::vector<Example> myExamples;
};
