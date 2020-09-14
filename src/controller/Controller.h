
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

    void run(Emulator* emulator, Agent* agent, bool agent_plays_first, int difficulty);

protected:

    bool waitFirstFrame();

    bool waitForIntroScreen();

    bool waitForMenu();

    bool processMenu();

    bool checkIntroScreen();

    void typeText(
        const char* text);

    void typeKeys(
        std::initializer_list<int> keys);

    std::string extractString(
        const cv::Mat4b& image,
        const cv::Vec4b& foreground=cv::Vec4b(255,255,255,255),
        const cv::Vec4b& background=cv::Vec4b(0,0,0,255));

protected:

    using ClockType = std::chrono::steady_clock;

protected:

    Emulator* myEmulator;
    Agent* myAgent;
    bool myAgentPlaysFirst;
    int myDifficulty;
    std::map<char, std::tuple<int,bool> > myCharToKey;
    std::map<uint64_t, std::string> myFont;
};

