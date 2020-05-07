#include <thread>
#include <map>
#include <SDL_keysym.h>
#include "agent.h"

void agent(Interface* interface)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::map<char,int> char_to_key;
    for(char i=0; i<25; i++)
    {
        char_to_key['a'+i] = SDLK_a+i;
    }
    char_to_key[' '] = SDLK_SPACE;
    char_to_key['\n'] = SDLK_RETURN;

    const char* cmd = "cd tombraid\ntomb\n";

    for(const char* p=cmd; *p != 0; p++)
    {
        interface->keyDown(char_to_key[*p]);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        interface->keyUp(char_to_key[*p]);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

