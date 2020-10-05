#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "Utils.h"
#include "MatchLog.h"

void MatchLog::fromBson(bsoncxx::document::view doc)
{
    agent_plays_first = doc["agent_plays_first"].get_bool();
    difficulty = doc["difficulty"].get_int32();
    result = doc["result"].get_int32();
    bsoncxx::document::view::iterator it_hostname = doc.find("hostname");
    if(it_hostname == doc.end())
    {
        hostname = "";
    }
    else
    {
        hostname = it_hostname->get_utf8();
    }
    agent = doc["agent"].get_utf8();
    start_timestamp = doc["start_timestamp"].get_date();

    bsoncxx::array::view arr = doc["agent_moves"].get_array();
    agent_moves.clear();
    for(bsoncxx::array::element item : arr)
    {
        bsoncxx::document::view item2 = item.get_document();

        agent_moves.emplace_back();

        agent_moves.back().timestamp_before_computation = item2["timestamp_before_computation"].get_date();
        agent_moves.back().timestamp_after_computation = item2["timestamp_after_computation"].get_date();

        const std::string_view grid_text = item2["state"].get_utf8();

        agent_moves.back().state.setMyTurn(true);
        agent_moves.back().state.setFlatGrid(std::string(grid_text).c_str());

        int num_cells = 0;
        int trajectory[Checkers::N];

        bsoncxx::array::view action_array = item2["action"].get_array();

        for(bsoncxx::array::element item3 : action_array)
        {
            trajectory[num_cells] = item3.get_int32();
            num_cells++;
        }

        agent_moves.back().action.set(num_cells-1, trajectory);
    }
}

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

    //builder << bsoncxx::builder::stream::finalize;

    return builder.extract();
}

void MatchLog::saveJson(const std::string& path) const
{
    std::ofstream f(path.c_str());
    f << bsoncxx::to_json(this->toBson());
}

