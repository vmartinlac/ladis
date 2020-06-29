
#pragma once

#include <map>
#include <vector>
#include <Checkers.h>
#include <QString>

class ActionDataset
{
protected:

    struct StateFrom
    {
        Checkers::State state;
        int offset;
        int count;
    };

    struct StateTo
    {
        Checkers::State state;
        float probability;
    };

public:

    bool load(const QString& path);
    void getExample(size_t index, Checkers::State& state_from, std::vector<Checkers::State>& state_to, std::vector<float>& transition_probability) const;
    size_t getNumExamples() const;

protected:

    void parseRootDirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);
    void parseSubdirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);
    void parseFile(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);

protected:

    std::vector<StateFrom> myStatesFrom;
    std::vector<StateTo> myStatesTo;
};
