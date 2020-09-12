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

        bool first = true;
        cv::Mat4b screen;
        cv::Mat4b newscreen;
        std::chrono::system_clock::time_point tscreen;

        interface->keyDown(LADIS_KEY_RIGHT);
        while( (std::chrono::system_clock::now() - t0) < std::chrono::milliseconds(60000) )
        {
            interface->keyDown(LADIS_KEY_b);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            interface->keyUp(LADIS_KEY_b);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            /*
            interface->keyDown(LADIS_KEY_v);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            interface->keyUp(LADIS_KEY_v);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            */

            std::chrono::system_clock::time_point tnewscreen = std::chrono::system_clock::now();
            //if( first || tnewscreen-tscreen > std::chrono::milliseconds(10) )
            {
                interface->getCurrentImage(newscreen);

                if( first || screen.size() != newscreen.size() )
                {
                    first = false;
                    tscreen = tnewscreen;
                    screen = newscreen;
                }
                else
                {
                    const double dt = 1.0e-6 * std::chrono::duration_cast<std::chrono::microseconds>(tnewscreen - tscreen).count();
                    //const double alpha = std::exp(-lambda*dt);
                    const double alpha = std::pow(0.5, dt/0.1);
                    screen = alpha * screen + (1.0-alpha)*newscreen;
                    tscreen = tnewscreen;
                }
            }

            const bool do_save = ( (std::chrono::system_clock::now() - tsave) > std::chrono::milliseconds(750) );
            if(do_save)
            {
                tsave = std::chrono::system_clock::now();
                std::stringstream fname;
                fname << "screen_" << id << ".png";
                cv::imwrite(fname.str(), screen);
                id++;
            }
        }
        interface->keyUp(LADIS_KEY_RIGHT);
    }
}

