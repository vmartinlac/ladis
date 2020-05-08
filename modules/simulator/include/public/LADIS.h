
#pragma once

#include <opencv2/core.hpp>
#include "LADIS_keys.h"

namespace LADIS
{

    class Interface
    {
    public:

        virtual void getCurrentImage(cv::Mat4b& image) = 0;
        virtual void mouseMove(int dx, int dy) = 0;
        virtual void mouseUp(int button) = 0;
        virtual void mouseDown(int button) = 0;
        virtual void keyUp(int key) = 0;
        virtual void keyDown(int key) = 0;
    };

    class Agent
    {
    public:

        virtual void play(Interface* interface) = 0;
    };

    void run(Agent* agent, bool with_ui = true);
}

