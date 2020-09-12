#include <iostream>
#include <opencv2/highgui.hpp>
#include <thread>
#include "Emulator.h"

int main(int num_args, char** args)
{
    std::vector<Emulator> em;

    em.resize(2);

    for(Emulator& e : em)
    {
        e.start();
    }

    for(int i=0; i<60; i++)
    {
        int j = 0;

        for(Emulator& e : em)
        {
            cv::Mat4b screen;
            e.readScreen(screen);


            if(screen.rows > 0 && screen.cols > 0 && screen.data)
            {
                //cv::imwrite("hello.png", screen);
                cv::imshow(std::to_string(j), screen);
                cv::waitKey(1);
            }
            else
            {
                std::cout << "Empty screen" << std::endl;
            }

            e.sendKeyDown(EMULATOR_KEY_a);
            e.sendKeyUp(EMULATOR_KEY_a);

            j++;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    for(Emulator& e : em)
    {
        e.stop();
    }

    return 0;
}

