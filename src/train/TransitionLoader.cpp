#include <iostream>
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include "TransitionLoader.h"

void TransitionLoader::load(TransitionList& transitions)
{
    mongocxx::client client(mongocxx::uri("mongodb://192.168.1.2/"));
    mongocxx::database db = client["ladis"];
    mongocxx::collection matches = db["matches"];

    //bsoncxx::builder::stream::document builder;
    //builder << "$limit" << 5;
    //mongocxx::cursor cursor = matches.find(builder.extract());

    mongocxx::pipeline pipeline;
    //pipeline.limit(5);
    mongocxx::cursor cursor = matches.aggregate(pipeline);

    transitions.clear();

    for(bsoncxx::document::view match : cursor)
    {
        MatchLog log;
        log.fromBson(match);

        std::cout << "Loading transitions form match " << match["_id"].get_oid().value.to_string() << std::endl;

        loadTransitionsFromMatch(log, transitions);
    }

    std::cout << "Number of samples: " << transitions.size() << std::endl;

    /*
    for(const std::pair<Checkers::Grid, Checkers::Grid>& transition : transitions)
    {
        std::cout << "-------------------------------------------------" << std::endl;

        Checkers::State s0;
        s0.setMyTurn(false);
        s0.setGrid(transition.first);

        Checkers::State s1;
        s1.setMyTurn(true);
        s1.setGrid(transition.second);

        std::cout << s0.getSquareGrid() << std::endl;
        std::cout << s1.getSquareGrid() << std::endl;

        std::cout << "-------------------------------------------------" << std::endl;
    }
    */
}

void TransitionLoader::loadTransitionsFromMatch(const MatchLog& match, TransitionList& transitions)
{
    bool has_last_state = false;
    Checkers::State last_state;

    if(!match.agent_plays_first)
    {
        has_last_state = true;
        last_state.setInitialGrid();
        last_state.setMyTurn(false);
    }

    Checkers::ActionIterator it;
    Checkers::Action available_action;
    Checkers::State available_state;

    for(const MatchLog::AgentMove& m : match.agent_moves)
    {

        if(has_last_state)
        {
            bool found = false;

            it.init(last_state);

            while(!found && it.next(last_state, available_action, available_state))
            {
                if(available_state == m.state)
                {
                    found = true;
                    transitions.insert(std::pair<Checkers::Grid, Checkers::Grid>(last_state.getGrid(), m.state.getGrid()));
                }
            }
        }

        {
            has_last_state = false;

            it.init(m.state);

            while(!has_last_state && it.next(last_state, available_action, available_state))
            {
                if(available_action == m.action)
                {
                    has_last_state = true;
                    last_state = available_state;
                }
            }
        }
    }
}

