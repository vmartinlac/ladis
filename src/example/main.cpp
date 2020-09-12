#include <iostream>
#include <opencv2/highgui.hpp>
#include <thread>
#include "Emulator.h"
#include "Controller.h"

class MyAgent : public Agent
{
public:

    void beginMatch() override
    {
    }

    void play(const std::string& checkerboard, std::vector<int>& move) override
    {
    }

    void endMatch(int result) override
    {
    }
};

int main(int num_args, char** args)
{
    Emulator em;
    MyAgent agent;
    Controller con;

    con.run(&em, &agent);

    return 0;
}

