
#pragma once

#include <fstream>
#include <opencv2/core.hpp>
#include "Checkers.h"

class GraphVizHook : public Checkers::Solver::Hook
{
public:

    GraphVizHook();
    void hookStarted() override;
    void hookFinished() override;
    void hookCompletedNode(const Checkers::State& s, int nodeid, float value) override;
    void hookFoundEdge(const Checkers::Action& action, int nodeid_from, int nodeid_to) override;

protected:

    cv::Mat1b makePicture(const Checkers::State& s);
    std::string makeNodeName(int nodeid);

protected:

    int myNextGraph;
    int myNextImage;
    std::ofstream myFile;
};

