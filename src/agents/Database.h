
#pragma once

#include "MatchLog.h"

class Database
{
public:

    virtual void saveMatch(const MatchLog& match) = 0;
};

