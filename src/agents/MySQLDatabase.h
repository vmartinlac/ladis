
#pragma once

#include "Database.h"
#include "mysql.h"

class MySQLDatabase : public Database
{
public:

    MySQLDatabase();

    ~MySQLDatabase();

    void saveMatch(const Match& match) override;

protected:

    MYSQL myConnection;
};

