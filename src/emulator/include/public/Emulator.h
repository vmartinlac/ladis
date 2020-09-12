
#pragma once

#include <opencv2/core.hpp>
#include "EmulatorKeycodes.h"

class Emulator
{
public:

    Emulator();
    ~Emulator();

    void start();
    void stop();
    void readScreen(cv::Mat4b& screen);
    void sendKeyDown(int key);
    void sendKeyUp(int key);

protected:

    void slaveLoop();

protected:

    static constexpr uint8_t CMD_QUIT = 0;
    static constexpr uint8_t CMD_EVENT = 1;
    static constexpr uint8_t CMD_READ_SCREEN = 2;

    bool myRunning;
    int myReceiver;
    int myTransmitter;
    int mySlavePid;

    static constexpr int MAX_WIDTH = 800;
    static constexpr int MAX_HEIGHT = 600;

    void* mySharedMemory;
};

