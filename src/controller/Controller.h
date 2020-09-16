
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

    enum Mode
    {
        MODE_TYPE_TRAJECTORY,
        MODE_WAIT_MY_TURN
    };

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

    void extractCell(
        const cv::Mat4b& screen,
        int no,
        cv::Mat4b& cell);

    void computeFeatures(
        const cv::Mat4b& picture,
        cv::Vec6f& features);

    char predictCell(
        const cv::Vec6f& features);

    void extractGrid(
        const cv::Mat4b& screen,
        Agent::State& grid);

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

