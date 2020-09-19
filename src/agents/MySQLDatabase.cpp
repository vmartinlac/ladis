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
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
            "start_datetime DATETIME,"
            "agent_plays_first INTEGER,"
            "difficulty INTEGER,"
            "result INTEGER,"
            "agent TEXT);";

    const char* schema1 =
        "CREATE TABLE IF NOT EXISTS agent_moves("
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
            "match_id INTEGER,"
            "rank INTEGER,"
            "grid CHAR(50),"
            "time_offset FLOAT,"
            "computation_time FLOAT);";

    const char* schema2 =
        "CREATE TABLE IF NOT EXISTS agent_atomic_moves("
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
            "move_id INTEGER,"
            "rank INTEGER,"
            "cell INTEGER);";

    const char* schema3 =
        "CREATE TABLE IF NOT EXISTS opponent_moves("
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
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
}

void MySQLDatabase::saveMatch(const Match& match)
{
    my_ulonglong match_id = 0;

    {
        MYSQL_BIND binds[5];
        memset(binds, 0, sizeof(MYSQL_BIND)*5);

        MYSQL_STMT* stmt = mysql_stmt_init(&myConnection);

        MYSQL_TIME start_datetime;
        {
            struct tm t;
            if(!localtime_r(&match.start_timestamp, &t))
            {
                std::cerr << "Could not retrieve local time!" << std::endl;
                exit(1);
            }

            start_datetime.year = t.tm_year + 1900;
            start_datetime.month = t.tm_mon;
            start_datetime.day = t.tm_mday;
            start_datetime.hour = t.tm_hour;
            start_datetime.minute = t.tm_min;
            start_datetime.second = t.tm_sec;
            std::cout << start_datetime.year << std::endl;
        }
        binds[0].buffer_type = MYSQL_TYPE_DATETIME;
        binds[0].buffer = &start_datetime;

        int agent_plays_first = int(match.agent_plays_first);
        binds[1].buffer_type = MYSQL_TYPE_LONG;
        binds[1].buffer = &agent_plays_first;

        int difficulty = match.difficulty;
        binds[2].buffer_type = MYSQL_TYPE_LONG;
        binds[2].buffer = &difficulty;

        int result = match.result;
        binds[3].buffer_type = MYSQL_TYPE_LONG;
        binds[3].buffer = &result;

        char agent[512];
        strncpy(agent, match.agent.c_str(), 512);
        unsigned long agent_length = strlen(agent);
        binds[4].buffer_type = MYSQL_TYPE_STRING;
        binds[4].buffer = agent;
        binds[4].length = &agent_length;

        if(mysql_stmt_prepare(stmt, "INSERT INTO matches(start_datetime, agent_plays_first, difficulty, result, agent) VALUES(?,?,?,?,?)", 0))
        {
            std::cerr << "Could not prepare MySQL statement!" << std::endl;
            exit(1);
        }

        if(mysql_stmt_bind_param(stmt, binds))
        {
            std::cerr << "Statement parameters binding failed!" << std::endl;
            exit(1);
        }

        if(mysql_stmt_execute(stmt))
        {
            std::cerr << mysql_stmt_error(stmt) << std::endl;
            std::cerr << "Error while executing MySQL statement!" << std::endl;
            exit(1);
        }

        if(mysql_stmt_close(stmt))
        {
            std::cerr << "Error while closing MySQL statement!" << std::endl;
            exit(1);
        }

        match_id = mysql_insert_id(&myConnection);
    }

    std::cerr << "saveMatch(...) not implemented!" << std::endl;
}

