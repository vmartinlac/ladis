#include <iostream>
#include <json/json.h>
#include "Utils.h"
#include "MatchLog.h"

void MatchLog::saveJson(const std::string& path)
{
    Json::Value agent_moves;

    for(const AgentMove& item : this->agent_moves)
    {
        if(!item.state.isMyTurn())
        {
            std::cerr << "Internal error" << std::endl;
            exit(1);
        }

        Json::Value action2;
        for(int i=0; i<item.action.getNumCells(); i++)
        {
            action2.append(item.action.getCell(i));
        }

        Json::Value item2;
        item2["state"] = item.state.getFlatGrid();
        item2["action"] = action2;
        item2["time_offset"] = item.time_offset;
        item2["computation_time"] = item.computation_time;

        agent_moves.append(item2);
    }

    Json::Value root;
    root["start_utc_datetime"] = Utils::makeUTCISO8601DateTime(this->start_timestamp);
    root["start_timestamp"] = Json::Int64(this->start_timestamp);
    root["agent_plays_first"] = this->agent_plays_first;
    root["difficulty"] = this->difficulty;
    root["result"] = this->result;
    root["agent"] = this->agent;
    root["agent_moves"] = agent_moves;

    {
        std::ofstream f(path.c_str());
        Json::StyledWriter writer;
        f << writer.write(root);
    }
}

