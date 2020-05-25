#include <thread>
#include <iostream>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "Agent.h"

TombRaiderAgent::TombRaiderAgent()
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

    myCharToKey[''] = std::make_tuple(LADIS_KEY_ESCAPE, false);
}

void TombRaiderAgent::typeText(LADIS::Interface* interface, const char* text)
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

void TombRaiderAgent::play(LADIS::Interface* interface)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    typeText(interface, "CD TOMBRAID\nTOMB\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    typeText(interface, "");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    typeText(interface, "");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    typeText(interface, "\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    typeText(interface, "\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    typeText(interface, "\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    interface->keyDown(LADIS_KEY_UP);

    int id = 0;
    const auto t0 = std::chrono::system_clock::now();
    while( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-t0).count() < 10*1000 )
    {
        std::stringstream s;
        s << "image_" << id << ".png";

        cv::Mat4b screen;
        interface->getCurrentImage(screen);

        cv::imwrite(s.str(), screen);
        id++;

        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}

