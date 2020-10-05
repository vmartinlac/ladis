
#pragma once

#include <memory>
#include <chrono>
#include <vector>
#include <bsoncxx/document/value.hpp>
#include "Checkers.h"

struct MatchLog
{
    struct AgentMove
    {
        Checkers::State state;
        Checkers::Action action;
        std::chrono::system_clock::time_point timestamp_before_computation;
        std::chrono::system_clock::time_point timestamp_after_computation;
    };

    std::chrono::system_clock::time_point start_timestamp;
    bool agent_plays_first;
    int difficulty;
    int result;
    std::string hostname;
    std::string agent;
    std::vector<AgentMove> agent_moves;

    void saveJson(const std::string& path) const;
    bsoncxx::document::value toBson() const;
    void fromBson(bsoncxx::document::view doc);
};

using MatchLogPtr = std::shared_ptr<MatchLog>;

