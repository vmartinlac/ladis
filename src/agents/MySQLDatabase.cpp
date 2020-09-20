#include <iostream>
#include <mariadb++/account.hpp>
#include "MySQLDatabase.h"

MySQLDatabase::~MySQLDatabase()
{
    myConnection.reset();
    myAccount.reset();
}

MySQLDatabase::MySQLDatabase()
{
    myAccount = mariadb::account::create("192.168.1.21", "ladis", "ladis", "ladis");
    myConnection = mariadb::connection::create(myAccount);

    const char* schema0 =
        "CREATE TABLE IF NOT EXISTS matches("
            "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
            "start_datetime DATETIME,"
            "agent_plays_first BOOLEAN,"
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
        myConnection->execute(*item);
    }
}

void MySQLDatabase::saveMatch(const Match& match)
{
    mariadb::statement_ref stmt0 = myConnection->create_statement("INSERT INTO matches(start_datetime, agent_plays_first, difficulty, result, agent) VALUES(?, ?, ?, ?, ?)");
    mariadb::statement_ref stmt1 = myConnection->create_statement("INSERT INTO agent_moves(match_id, rank, grid, time_offset, computation_time) VALUES(?,?,?,?,?)");
    mariadb::statement_ref stmt2 = myConnection->create_statement("INSERT INTO agent_atomic_moves(move_id, rank, cell) VALUES(?,?,?)");

    stmt0->set_date_time(0, mariadb::date_time(match.start_timestamp));
    stmt0->set_boolean(1, match.agent_plays_first);
    stmt0->set_signed32(2, match.difficulty);
    stmt0->set_signed32(3, match.result);
    stmt0->set_string(4, match.agent);
    const mariadb::u64 match_id = stmt0->insert();

    int move_index = 0;
    for(const AgentMove& m : match.agent_moves)
    {
        stmt1->set_unsigned64(0, match_id);
        stmt1->set_signed32(1, move_index);
        stmt1->set_string(2, m.state.getFlatGrid());
        stmt1->set_double(3, m.time_offset);
        stmt1->set_double(4, m.computation_time);
        const mariadb::u64 move_id = stmt1->insert();

        for(int cell_index=0; cell_index<m.action.getNumCells(); cell_index++)
        {
            stmt2->set_unsigned64(0, move_id);
            stmt2->set_signed32(1, cell_index);
            stmt2->set_signed32(2, m.action.getCell(cell_index));
            stmt2->insert();
        }

        move_index++;
    }
}

