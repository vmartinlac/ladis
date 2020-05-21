
#pragma once

#include <map>
#include <LADIS.h>
#include "Checkers.h"

class DamesAgent : public LADIS::Agent
{
public:

    DamesAgent();

    void play(LADIS::Interface* interface) override;

protected:

    void typeText(LADIS::Interface* interface, const char* text);
    void extractCell(const cv::Mat4b& screen, int no, cv::Mat4b& cell);
    void computeFeatures(const cv::Mat4b& roi, cv::Vec6f& features);
    char predictCell(const cv::Vec6f& features);
    void readState(const cv::Mat4b& screen, Checkers::State& s);

protected:

    std::map<char, std::tuple<int,bool> > myCharToKey;
};

