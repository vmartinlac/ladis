
#pragma once

#include <map>
#include <tuple>
#include <chrono>
#include "Emulator.h"
#include "Agent.h"

class Controller
{
public:

    Controller();

    void run(Emulator* emulator, Agent* agent);

protected:

    bool waitFirstFrame(cv::Mat4b& screen);
    bool waitForIntroScreen();
    bool waitForMainScreen();
    bool checkIntroScreen();
    void typeText(const char* text);

protected:

    using ClockType = std::chrono::steady_clock;

protected:

    Emulator* myEmulator;
    Agent* myAgent;
    std::map<char, std::tuple<int,bool> > myCharToKey;
};

