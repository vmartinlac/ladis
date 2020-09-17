
#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include "CheckersBase.h"
#include "sqlite3.h"

class Database
{
public:

    Database(const std::string database_path);
    Database(Database&& o) = delete;
    Database(const Database& o) = delete;

    ~Database();

    struct Move
    {
        char grid[CheckersBase::N];
        std::vector<int> moves;
    };

    struct Match
    {
        std::string start_datetime;
        bool agent_plays_first;
        int difficulty;
        int result;
        std::string agent;
        std::vector<Move> moves;
    };

    void saveGame(const Match& match);

protected:

    std::mutex myMutex;
    sqlite3* myConnection;
    sqlite3_stmt* myInsertMatchStmt;
    sqlite3_stmt* myInsertMoveStmt;
};

