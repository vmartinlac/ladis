#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <thread>
#include <opencv2/core.hpp>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <LADIS.h>

class ScreenInterpreter
{
public:

    void analyze(const cv::Mat3b& screen);
};

class StateMachine
{
public:

    enum
    {
        E_OK,
        E_EXIT,
        E_ERROR
    };

    StateMachine()
    {
    }

    int step(
        const cv::Mat3b& image,
        std::string& text,
        double& duration)
    {
        myInterpreter.analyze(image);
        return 0;
    }

protected:

    enum
    {
        S_NONE
    };

protected:

    int myLastState;
    ScreenInterpreter myInterpreter;
};

class DamesAgent : public LADIS::Agent
{
protected:

    std::map<char, std::tuple<int,bool> > myCharToKey;

public:

    DamesAgent()
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

    void play(LADIS::Interface* interface) override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        typeText(interface, "CD DAME2020\nDA2020\n");

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        typeText(interface, "\n");

        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }

    void typeText(LADIS::Interface* interface, const char* text)
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
};

void run_simulation(int simulation_id)
{
    std::stringstream directory_ss;
    directory_ss << "simulation_" << std::setw(5) << std::setfill('0') << simulation_id;

    const std::string directory = directory_ss.str();

    if( mkdir(directory.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) < 0 )
    {
        // FIXME
        //throw std::runtime_error("Could not create directory!");
    }

    if( chdir(directory.c_str()) < 0 )
    {
        throw std::runtime_error("Could not change current directory!");
    }

    std::ofstream log("log.txt");
    log << simulation_id << std::endl;

    // TODO: run the game!
    DamesAgent agent;
    LADIS::run(&agent, true);
}

int main(int num_args, char** args)
{
    if(num_args != 3)
    {
        throw std::runtime_error("Bad number of arguments!");
    }

    const int num_simulations = atoi(args[1]);
    const int num_processes = atoi(args[2]);

    if(num_simulations < 0 || num_processes < 0)
    {
        throw std::runtime_error("Bad input values");
    }

    int next_simulation = 0;
    int running = 0;

    bool go_child = false;
    while(!go_child && ( running > 0 || next_simulation < num_simulations ))
    {
        if(running >= num_processes || next_simulation >= num_simulations)
        {
            int status;
            if( wait(&status) >= 0 )
            //if( wait(&status) >= 0 && WIFEXITED(status) )
            {
                running--;
            }
        }
        else
        {
            if(fork() == 0)
            {
                go_child = true;
            }
            else
            {
                running++;
                next_simulation++;
            }
        }
    }

    if(go_child)
    {
        run_simulation(next_simulation);
    }

    return 0;
}

