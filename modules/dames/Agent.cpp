#include <thread>
#include <iostream>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "Agent.h"

DamesAgent::DamesAgent()
{
    // initialize table of character to key code.

    for(char i=0; i<25; i++)
    {
        myCharToKey['a'+i] = std::make_tuple(LADIS_KEY_a+i, false);
        myCharToKey['A'+i] = std::make_tuple(LADIS_KEY_a+i, true);
    }

    for(char i=0; i<10; i++)
    {
        myCharToKey['0'+i] = std::make_tuple(LADIS_KEY_0 + i, false);
    }

    myCharToKey[' '] = std::make_tuple(LADIS_KEY_SPACE, false);
    myCharToKey['\n'] = std::make_tuple(LADIS_KEY_RETURN, false);

    myCharToKey['+'] = std::make_tuple(LADIS_KEY_KP_PLUS, false);
    myCharToKey['-'] = std::make_tuple(LADIS_KEY_KP_MINUS, false);
    myCharToKey['*'] = std::make_tuple(LADIS_KEY_KP_MULTIPLY, false);
    myCharToKey['/'] = std::make_tuple(LADIS_KEY_KP_DIVIDE, false);

}

void DamesAgent::typeText(LADIS::Interface* interface, const char* text)
{
    const int delay = 80;
    bool maj = false;

    for(const char* p=text; *p != 0; p++)
    {
        if( myCharToKey.find(*p) == myCharToKey.end() )
        {
            std::cout << "Character not found!" << std::endl;
        }

        const int key = std::get<0>(myCharToKey[*p]);
        const bool maj_required = std::get<1>(myCharToKey[*p]);

        if(maj_required && !maj)
        {
            maj = true;
            interface->keyDown(LADIS_KEY_LSHIFT);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        else if(!maj_required && maj)
        {
            maj = false;
            interface->keyUp(LADIS_KEY_LSHIFT);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        interface->keyDown(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        interface->keyUp(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    if(maj)
    {
        interface->keyUp(LADIS_KEY_LSHIFT);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void DamesAgent::play(LADIS::Interface* interface)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    typeText(interface, "CD DAME2020\nDA2020\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    typeText(interface, "\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    typeText(interface, "5\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    cv::Mat4b im;
    interface->getCurrentImage(im);
    cv::imwrite("tmp.png", im);
}

