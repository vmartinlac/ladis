
#pragma once

#include <map>
#include <LADIS.h>
#include "Checkers.h"

class DamesAgent : public LADIS::Agent
{
public:

    enum Outcome
    {
        OUTCOME_WIN,
        OUTCOME_LOSE,
        OUTCOME_DRAW,
        OUTCOME_OTHER
    };

    DamesAgent();

    void setSaveScreens(bool value);
    void setOpponentSkill(int skill);
    void setMinimaxDepth(int depth);

    void play(LADIS::Interface* interface) override;

    Outcome getOutcome();

protected:

    void typeText(LADIS::Interface* interface, const char* text);
    void extractCell(const cv::Mat4b& screen, int no, cv::Mat4b& cell);
    void computeFeatures(const cv::Mat4b& roi, cv::Vec6f& features);
    char predictCell(const cv::Vec6f& features);
    void readState(const cv::Mat4b& screen, Checkers::State& s);
    bool extractIsMyTurn(const cv::Mat4b& screen);
    void saveScreen(const cv::Mat4b& screen);
    bool tryExtractOutcome(const cv::Mat4b& screen);
    bool compareGrayscale(const cv::Mat4b& m0, const cv::Mat1b& m1);

protected:

    std::map<char, std::tuple<int,bool> > myCharToKey;
    int myScreenshotCount;
    int myOpponentSkill;
    Outcome myOutcome;
    bool mySaveScreens;
    int myMinimaxDepth;
};

