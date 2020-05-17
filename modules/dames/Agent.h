
#pragma once

#include <map>
#include <LADIS.h>

class DamesAgent : public LADIS::Agent
{
public:

    DamesAgent();

    void play(LADIS::Interface* interface) override;

protected:

    void typeText(LADIS::Interface* interface, const char* text);

protected:

    std::map<char, std::tuple<int,bool> > myCharToKey;
};

