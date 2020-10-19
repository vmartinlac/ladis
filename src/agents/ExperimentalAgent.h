
#pragma once

#include <random>
#include <memory>
#include <map>
#include <bsoncxx/document/view.hpp>
#include <Checkers.h>
#include <Agent.h>

class ExperimentalAgent : public Agent
{
public:

    ExperimentalAgent();

    std::string getName() final;

    void beginMatch(bool agent_plays_first, int difficulty) final;

    void endMatch(int result) final;

    bool play(const Checkers::State& state, Checkers::Action& action) final;

protected:

    double computeValue(const Checkers::State& state, int depth);

    double computeHeuristicValue(const Checkers::State& state);

    void learnMatch(bsoncxx::document::view doc);

protected:

    std::multimap<Checkers::Grid,Checkers::Grid> myOpponentSamples;
    int myMaxDepth;
    std::default_random_engine myRNG;
};

