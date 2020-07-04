
#pragma once

#include <map>
#include <vector>
#include <Checkers.h>
#include <QString>

class LogLoader
{
protected:

    struct StateFrom
    {
        Checkers::State state;
        //int offset;
        //int count;
    };

    struct StateTo
    {
        int index_from;
        Checkers::State state;
        float probability;
    };

public:

    bool load(const QString& path);
    size_t getNumExamples() const;
    void getExample(size_t index, Checkers::State& state_from, Checkers::State& state_to, float& transition_probability) const;

protected:

    void parseRootDirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);
    void parseSubdirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);
    void parseFile(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen);

protected:

    std::vector<StateFrom> myStatesFrom;
    std::vector<StateTo> myStatesTo;
};
