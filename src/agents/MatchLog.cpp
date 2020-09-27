#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "Utils.h"
#include "MatchLog.h"

bsoncxx::document::value MatchLog::toBson() const
{
    bsoncxx::builder::stream::array agent_moves;

    for(const AgentMove& item : this->agent_moves)
    {
        if(!item.state.isMyTurn())
        {
            std::cerr << "Internal error" << std::endl;
            exit(1);
        }

        bsoncxx::builder::stream::array trajectory_builder;
        for(int i=0; i<item.action.getNumCells(); i++)
        {
            trajectory_builder << item.action.getCell(i);
        }

        bsoncxx::builder::stream::document move_builder;
        move_builder << "state" << item.state.getFlatGrid();
        move_builder << "action" << trajectory_builder.extract();
        move_builder << "timestamp_before_computation" << bsoncxx::types::b_date(item.timestamp_before_computation);
        move_builder << "timestamp_after_computation" << bsoncxx::types::b_date(item.timestamp_after_computation);

        agent_moves << move_builder.extract();
    }

    bsoncxx::builder::stream::document builder;

    builder << "start_timestamp" << bsoncxx::types::b_date(this->start_timestamp);
    builder << "agent_plays_first" << this->agent_plays_first;
    builder << "difficulty" << this->difficulty;
    builder << "result" << this->result;
    builder << "agent" << this->agent;
    builder << "agent_moves" << agent_moves.extract();

    builder << bsoncxx::builder::stream::finalize;

    return builder.extract();
}

void MatchLog::saveJson(const std::string& path) const
{
    std::ofstream f(path.c_str());
    f << bsoncxx::to_json(this->toBson());
}

