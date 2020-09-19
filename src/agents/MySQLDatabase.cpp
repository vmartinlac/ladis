#include <iostream>
#include "MySQLDatabase.h"

MySQLDatabase::~MySQLDatabase()
{
    mysql_close(&myConnection);
}

MySQLDatabase::MySQLDatabase()
{
    mysql_init(&myConnection);
    if(!mysql_real_connect(&myConnection, nullptr, "ladis", "ladis", "ladis", 0, nullptr, 0))
    {
        std::cerr << "Could not connect to mysql database!" << std::endl;
        exit(1);
    }

    const char* schema0 =
        "CREATE TABLE IF NOT EXISTS matches("
            "id INTEGER PRIMARY KEY,"
            "start_datetime DATETIME,"
            "agent_plays_first INTEGER,"
            "difficulty INTEGER,"
            "result INTEGER,"
            "agent TEXT);";

    const char* schema1 =
        "CREATE TABLE IF NOT EXISTS agent_moves("
            "id INTEGER PRIMARY KEY,"
            "match_id INTEGER,"
            "rank INTEGER,"
            "grid CHAR(50),"
            "time_offset FLOAT,"
            "computation_time FLOAT);";

    const char* schema2 =
        "CREATE TABLE IF NOT EXISTS agent_atomic_moves("
            "id INTEGER PRIMARY KEY,"
            "move_id INTEGER,"
            "rank INTEGER,"
            "cell INTEGER);";

    const char* schema3 =
        "CREATE TABLE IF NOT EXISTS opponent_moves("
            "id INTEGER PRIMARY KEY,"
            "match_id INTEGER,"
            "grid_before CHAR(50),"
            "grid_after CHAR(50));";

    const char* prelude[] = { schema0, schema1, schema2, schema3, nullptr };

    for(const char** item=prelude; *item!=nullptr; item++)
    {
        if(mysql_query(&myConnection, *item))
        {
            std::cout << "Failed to initialize connection to MySQL!" << std::endl;
            exit(1);
        }
    }

    /*
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
    */
}

void MySQLDatabase::saveMatch(const Match& match)
{
    std::cerr << "saveMatch(...) not implemented!" << std::endl;
    //abort();
}

