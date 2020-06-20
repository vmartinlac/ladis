
#pragma once

#include <string>
#include "Checkers.h"

class StateSequence
{
public:

    static bool load(const std::string& path, std::vector<Checkers::State>& sequence);
    static bool save(const std::vector<Checkers::State>& sequence, const std::string& path);
};

