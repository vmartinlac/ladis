
#pragma once

#include <map>
#include <Checkers.h>
#include "Agent.h"

class Arena
{
public:

    enum Outcome
    {
        OUTCOME_WIN,
        OUTCOME_LOSE,
        OUTCOME_DRAW,
        OUTCOME_ILLEGAL_AGENT_MOVE,
        OUTCOME_ILLEGAL_OPPONENT_MOVE,
        OUTCOME_ERROR
    };

    static Arena& getInstance();

    void start();
    void quit();

    void setMaxNumTurns(int value);
    void setSaveScreens(bool value);
    void setOpponentSkill(int skill);
    void setAgentStarts(bool value);

    void play(Agent* agent);

    const std::vector<Checkers::State>& refLog() const;

    Outcome getOutcome();

protected:

    Arena();
    void typeText(const char* text);
    void pressKey(int key);
    void extractCell(const cv::Mat4b& screen, int no, cv::Mat4b& cell);
    void computeFeatures(const cv::Mat4b& roi, cv::Vec6f& features);
    char predictCell(const cv::Vec6f& features);
    void readState(const cv::Mat4b& screen, Checkers::State& s);
    bool extractIsMyTurn(const cv::Mat4b& screen);
    void saveScreen(const cv::Mat4b& screen);
    bool tryExtractOutcome(const cv::Mat4b& screen);
    bool compareGrayscale(const cv::Mat4b& m0, const cv::Mat1b& m1);

protected:

    static std::unique_ptr<Arena> myInstance;
    std::map<char, std::tuple<int,bool> > myCharToKey;
    int myScreenshotCount;
    int myOpponentSkill;
    Outcome myOutcome;
    bool mySaveScreens;
    bool myAgentStarts;
    int myMaxNumTurns;
    std::vector<Checkers::State> myLog;
};

