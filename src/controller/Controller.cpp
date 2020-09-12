#include <iostream>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include "Controller.h"

Controller::Controller()
{
    for(char i=0; i<25; i++)
    {
        myCharToKey['a'+i] = std::make_tuple(EMULATOR_KEY_a+i, false);
        myCharToKey['A'+i] = std::make_tuple(EMULATOR_KEY_a+i, true);
    }

    for(char i=0; i<10; i++)
    {
        myCharToKey['0'+i] = std::make_tuple(EMULATOR_KEY_0 + i, false);
    }

    myCharToKey[' '] = std::make_tuple(EMULATOR_KEY_SPACE, false);
    myCharToKey['\n'] = std::make_tuple(EMULATOR_KEY_RETURN, false);

    myCharToKey[':'] = std::make_tuple(EMULATOR_KEY_SEMICOLON, true);

    myCharToKey['+'] = std::make_tuple(EMULATOR_KEY_KP_PLUS, false);
    myCharToKey['-'] = std::make_tuple(EMULATOR_KEY_KP_MINUS, false);
    myCharToKey['*'] = std::make_tuple(EMULATOR_KEY_KP_MULTIPLY, false);
    myCharToKey['/'] = std::make_tuple(EMULATOR_KEY_KP_DIVIDE, false);
}

void Controller::run(Emulator* emulator, Agent* agent)
{
    myEmulator = emulator;
    myAgent = agent;

    cv::Mat4b screen;
    int result = Agent::RESULT_ERROR;
    bool go_on = true;

    agent->beginMatch();
    emulator->start();

    if(go_on)
    {
        go_on = waitFirstFrame(screen);
    }

    if(go_on)
    {
        typeText("mount c /DATA/victor/emulation/dosbox\n");
        typeText("c:\n");
        typeText("cd DAME2020\n");
        typeText("DA2020\n");

        go_on = waitForIntroScreen();
    }

    if(go_on)
    {
        typeText("\n");

        go_on = waitForMainScreen();
    }

    emulator->stop();
    agent->endMatch(result);
}

bool Controller::waitFirstFrame(cv::Mat4b& screen)
{
    const ClockType::time_point t0 = ClockType::now();

    myEmulator->readScreen(screen);

    while( std::chrono::duration_cast<std::chrono::milliseconds>(ClockType::now() - t0).count() < 1000 && screen.size() == cv::Size(0,0))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        myEmulator->readScreen(screen);
    }

    return screen.size() != cv::Size(0, 0);
}

bool Controller::checkIntroScreen()
{
    const cv::Vec4b refcolor(0xaa, 0xaa, 0x00, 0xFF);
    cv::Mat4b screen;

    bool ret = false;

    myEmulator->readScreen(screen);

    if(screen.size() == cv::Size(640, 480))
    {
        const int margin = 10;

        std::array<cv::Mat4b,2> ROI;
        ROI[0] = screen(cv::Rect(0, 0, margin, 480));
        ROI[1] = screen(cv::Rect(640-margin, 0, margin, 480));

        ret = true;
        for(cv::Mat4b& item : ROI)
        {
            ret = ret && std::all_of(item.begin(), item.end(), [refcolor] (const cv::Vec4b& x) { return x == refcolor; });
        }
    }

    return ret;
}

bool Controller::waitForIntroScreen()
{
    const ClockType::time_point t0 = ClockType::now();

    bool ret = checkIntroScreen();

    while( !ret && std::chrono::duration_cast<std::chrono::milliseconds>(ClockType::now() - t0).count() < 2000 )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ret = checkIntroScreen();
    }

    return ret;
}

bool Controller::waitForMainScreen()
{
    cv::Mat4b screen;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    myEmulator->readScreen(screen);
    cv::imwrite("hello.png", screen);

    // TODO

    return false;
}

void Controller::typeText(const char* text)
{
    const int delay = 10;
    bool maj = false;

    for(const char* p=text; *p != 0; p++)
    {
        if( myCharToKey.find(*p) == myCharToKey.end() )
        {
            throw std::runtime_error("Character not found!");
        }

        const int key = std::get<0>(myCharToKey[*p]);
        const bool maj_required = std::get<1>(myCharToKey[*p]);

        if(maj_required && !maj)
        {
            maj = true;
            myEmulator->sendKeyDown(EMULATOR_KEY_LSHIFT);
            if(delay > 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }
        else if(!maj_required && maj)
        {
            maj = false;
            myEmulator->sendKeyUp(EMULATOR_KEY_LSHIFT);
            if(delay > 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }

        myEmulator->sendKeyDown(key);

        if(delay > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        myEmulator->sendKeyUp(key);

        if(delay > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    if(maj)
    {
        myEmulator->sendKeyUp(EMULATOR_KEY_LSHIFT);

        if(delay > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }
}
