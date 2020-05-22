#include <thread>
#include <opencv2/imgcodecs.hpp>
#include <map>
#include "Agent.h"

void AsterixAgent::play(LADIS::Interface* interface)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::map<char,int> char_to_key;
    for(char i=0; i<25; i++)
    {
        char_to_key['a'+i] = LADIS_KEY_a+i;
    }
    char_to_key[' '] = LADIS_KEY_SPACE;
    char_to_key['\n'] = LADIS_KEY_RETURN;

    //const char* cmd = "cd tombraid\ntomb\n";
    const char* cmd = "cd asterix\nobelix\n";

    for(const char* p=cmd; *p != 0; p++)
    {
        interface->keyDown(char_to_key[*p]);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        interface->keyUp(char_to_key[*p]);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    {
        auto t0 = std::chrono::system_clock::now();

        while( (std::chrono::system_clock::now() - t0) < std::chrono::milliseconds(20000) )
        {
            interface->keyDown(LADIS_KEY_RETURN);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            interface->keyUp(LADIS_KEY_RETURN);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    {
        int id = 0;
        auto tsave = std::chrono::system_clock::now();

        auto t0 = std::chrono::system_clock::now();

        interface->keyDown(LADIS_KEY_RIGHT);
        while( (std::chrono::system_clock::now() - t0) < std::chrono::milliseconds(60000) )
        {
            interface->keyDown(LADIS_KEY_b);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            interface->keyUp(LADIS_KEY_b);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            /*
            interface->keyDown(LADIS_KEY_v);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            interface->keyUp(LADIS_KEY_v);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            */

            const bool do_save = ( (std::chrono::system_clock::now() - tsave) > std::chrono::milliseconds(2000) );
            if(do_save)
            {
                tsave = std::chrono::system_clock::now();
                std::stringstream fname;
                fname << "screen_" << id << ".png";
                cv::Mat4b screen;
                interface->getCurrentImage(screen);
                cv::imwrite(fname.str(), screen);
                id++;
            }
        }
        interface->keyUp(LADIS_KEY_RIGHT);
    }
}

