
#pragma once

#include "MatchLog.h"
#include "TransitionList.h"

class TransitionLoader
{
public:

    static void load(TransitionList& transitions);

protected:

    static void loadTransitionsFromMatch(const MatchLog& match, TransitionList& transitions);
};

