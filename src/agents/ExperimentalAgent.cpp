#include <iostream>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "ExperimentalAgent.h"

ExperimentalAgent::ExperimentalAgent()
{
    myMaxDepth = 7;
}

std::string ExperimentalAgent::getName()
{
    return "experimental";
}

void ExperimentalAgent::beginMatch(bool agent_plays_first, int difficulty)
{
    mongocxx::client client(mongocxx::uri("mongodb://192.168.1.2/"));
    mongocxx::database db = client["ladis"];
    mongocxx::collection matches = db["matches"];

    bsoncxx::builder::stream::document builder;
    builder
        << "difficulty" << 3
        << "result"
        << bsoncxx::builder::stream::open_document
        << "$in"
        << bsoncxx::builder::stream::open_array
        << 0
        << 1
        << bsoncxx::builder::stream::close_array
        << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value mask = builder.extract();

    myOpponentSamples.clear();

    mongocxx::cursor cursor = matches.find(std::move(mask));
    for(bsoncxx::document::view doc : cursor)
    {
        learnMatch(doc);
    }
}

void ExperimentalAgent::learnMatch(bsoncxx::document::view doc)
{
    // TODO
}

void ExperimentalAgent::endMatch(int result)
{
    myOpponentSamples.clear();
}

bool ExperimentalAgent::play(const Checkers::State& state, Checkers::Action& action)
{
    bool ret = false;

    if(state.isMyTurn())
    {
        Checkers::ActionIterator it;
        it.init(state);

        Checkers::Action investigated_action;
        Checkers::State investigated_state;
        double best_value = 0.0;

        while(it.next(state, investigated_action, investigated_state))
        {
            const double investigated_value = computeValue(investigated_state, 0);

            if(!ret || investigated_value > best_value)
            {
                ret = true;
                best_value = investigated_value;
                action = investigated_action;
            }
        }
    }

    return ret;
}

double ExperimentalAgent::computeValue(const Checkers::State& state, int depth)
{
    double ret = 0.0;

    if(depth >= myMaxDepth)
    {
        ret = computeHeuristicValue(state);
    }
    else if(state.isMyTurn())
    {
        Checkers::ActionIterator it;

        it.init(state);

        Checkers::Action unused0;
        Checkers::State new_state;

        bool has_action = false;

        ret = -1.0;

        while(it.next(state, unused0, new_state))
        {
            ret = std::max<double>(ret, computeValue(new_state, depth+1));
        }
    }
    else
    {
        Checkers::ActionIterator it;

        it.init(state);

        Checkers::Action unused0;
        Checkers::State new_state;

        double ret_min = 1.0;
        //double ret_mean = 1.0;

        while(it.next(state, unused0, new_state))
        {
            ret_min = std::min<double>(ret_min, computeValue(new_state, depth+1));
        }

        ret = ret_min;
    }

    return 0.0;
}

double ExperimentalAgent::computeHeuristicValue(const Checkers::State& state)
{
    // must return value between -1.0 and 1.0.

    int players_men = 0;
    int players_queens = 0;
    int opponents_men = 0;
    int opponents_queens = 0;

    for(int i=0; i<Checkers::N; i++)
    {
        switch(state.readCell(i))
        {
        case 'p':
            players_men++;
            break;
        case 'P':
            players_queens++;
            break;
        case 'o':
            opponents_men++;
            break;
        case 'O':
            opponents_queens++;
            break;
        }
    }

    constexpr double gamma = 2.0;
    constexpr double alpha = 0.1;
    const double value = alpha * ( (players_men - opponents_men) + gamma * (players_queens - opponents_queens) ) / (40.0 * (1.0 + gamma));

    return value;
}

