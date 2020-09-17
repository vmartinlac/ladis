#include <iostream>
#include "Database.h"

Database::Database(const std::string database_path)
{
    const char* schema0 =
        "CREATE TABLE IF NOT EXISTS matches("
            "id INTEGER PRIMARY KEY,"
            "start_datetime TEXT,"
            "agent_plays_first INTEGER,"
            "difficulty INTEGER,"
            "result INTEGER,"
            "agent TEXT);";

    const char* schema1 =
        "CREATE TABLE IF NOT EXISTS moves("
            "id INTEGER PRIMARY KEY,"
            "match_id INTEGER,"
            "rank INTEGER,"
            "state TEXT,"
            "action TEXT,"
            "time_offset FLOAT,"
            "computation_time FLOAT);";

    const char* prelude[] = { schema0, schema1, nullptr };

    if( sqlite3_open(database_path.c_str(), &myConnection) != SQLITE_OK )
    {
        std::cerr << "Could not connect to database!" << std::endl;
        abort();
    }

    for(const char** item=prelude; *item!=nullptr; item++)
    {
        char* errmsg = nullptr;

        if( sqlite3_exec(myConnection, *item, nullptr, nullptr, &errmsg) != SQLITE_OK )
        {
            std::cerr << errmsg << std::endl;
            sqlite3_free(errmsg);
            abort();
        }
    }

    const char* sql_stmt[] = {
        "INSERT INTO matches(start_datetime,agent_plays_first,difficulty,result,agent) VALUES(?,?,?,?,?)",
        "INSERT INTO moves(match_id,rank,state,action,time_offset,computation_time) VALUES(?,?,?,?,?,?)",
        nullptr
    };

    sqlite3_stmt** ptr_stmt[] = {
        &myInsertMatchStmt,
        &myInsertMoveStmt,
        nullptr
    };

    const char** item_sql = sql_stmt;
    sqlite3_stmt*** item_ptr = ptr_stmt;
    while(*item_sql && *item_ptr)
    {
        if( sqlite3_prepare_v2(myConnection, *item_sql, -1, *item_ptr, nullptr) != SQLITE_OK )
        {
            std::cout << "Could not prepare statement!" << std::endl;
            abort();
        }

        item_sql++;
        item_ptr++;
    }
}

void Database::saveGame(const Match& match)
{
    std::lock_guard<std::mutex> lock(myMutex);

    // TODO
    /*
    sqlite3_bind_text(myInsertMatchStmt, 1, 
    for(int i=0; i<match.moves.size(); i++)
    {
        sqlite3_bind_
    }
    */
}

Database::~Database()
{
    sqlite3_finalize(myInsertMatchStmt);
    sqlite3_finalize(myInsertMoveStmt);
    sqlite3_close(myConnection);
}

