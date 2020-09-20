
#pragma once

#include <mariadb++/account.hpp>
#include <mariadb++/connection.hpp>
#include "Database.h"

class MySQLDatabase : public Database
{
public:

    MySQLDatabase();

    ~MySQLDatabase();

    void saveMatch(const MatchLog& match) override;

protected:

    mariadb::account_ref myAccount;
    mariadb::connection_ref myConnection;
};

