
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

void run_simulation(int simulation_id)
{
    std::stringstream directory_ss;
    directory_ss << "simulation_" << std::setw(5) << std::setfill('0') << simulation_id;

    const std::string directory = directory_ss.str();

    if( mkdir(directory.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) < 0 )
    {
        throw std::runtime_error("Could not create directory!");
    }

    if( chdir(directory.c_str()) < 0 )
    {
        throw std::runtime_error("Could not change current directory!");
    }

    std::ofstream log("log.txt");
    log << simulation_id << std::endl;

    // TODO: run the game!
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

