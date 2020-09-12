#include <queue>
#include <thread>
#include <iostream>
#include <cassert>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "Emulator.h"
#include "EmulatorInternals.h"

int DOSBOX_main();

static std::mutex emulator_screen_mutex;
static cv::Mat4b emulator_screen;
static std::mutex emulator_event_queue_mutex;
static std::queue<EmulatorEvent> emulator_event_queue;

Emulator::Emulator()
{
    myRunning = false;
}

Emulator::~Emulator()
{
    if(myRunning)
    {
        stop();
        myRunning = false;
    }
}

void Emulator::start()
{
    int master_to_slave[2];
    int slave_to_master[2];

    if(pipe(master_to_slave) != 0 || pipe(slave_to_master) != 0)
    {
        std::cerr << "Could not create pipes!" << std::endl;
        abort();
    }

    mySharedMemory = mmap(nullptr, 4*MAX_WIDTH*MAX_HEIGHT, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    mySlavePid = fork();

    myRunning = true;

    if(mySlavePid == 0)
    {
        close(master_to_slave[1]);
        close(slave_to_master[0]);
        myTransmitter = slave_to_master[1];
        myReceiver = master_to_slave[0];

        slaveLoop();
    }
    else
    {
        close(master_to_slave[0]);
        close(slave_to_master[1]);
        myReceiver = slave_to_master[0];
        myTransmitter = master_to_slave[1];
    }
}

void Emulator::slaveLoop()
{
    std::thread dosbox_thread(DOSBOX_main);

    while(true)
    {
        uint8_t cmd;
        if( read(myReceiver, &cmd, 1) != 1 )
        {
            std::cerr << "Slave could not read from pipe!" << std::endl;
            abort();
        }

        switch(cmd)
        {

        case CMD_EVENT:
            {
                EmulatorEvent event;

                if( read(myReceiver, &event, sizeof(EmulatorEvent)) != sizeof(EmulatorEvent) )
                {
                    std::cerr << "Piped communication failed!" << std::endl;
                    abort();
                }

                {
                    std::lock_guard<std::mutex> lock(emulator_event_queue_mutex);
                    emulator_event_queue.push(event);
                }
            }
            break;

        case CMD_READ_SCREEN:
            {
                int rows = 0;
                int cols = 0;

                {
                    std::lock_guard<std::mutex> lock(emulator_screen_mutex);

                    assert(emulator_screen.isContinuous());

                    rows = emulator_screen.rows;
                    cols = emulator_screen.cols;

                    if(cols > MAX_WIDTH || rows > MAX_HEIGHT)
                    {
                        std::cerr << "Screen too large for buffer!" << std::endl;
                        abort();
                    }

                    memcpy(mySharedMemory, emulator_screen.ptr(), 4*rows*cols);
                }

                write(myTransmitter, &rows, sizeof(int));
                write(myTransmitter, &cols, sizeof(int));
            }
            break;

        case CMD_QUIT:
            exit(0);
            break;

        default:
            std::cerr << "Slave received unknown command!" << std::endl;
            abort();
            break;
        }
    }
}

void Emulator::stop()
{
    const uint8_t cmd = CMD_QUIT;

    if(write(myTransmitter, &cmd, 1) != 1)
    {
        std::cerr << "Master could not send command!" << std::endl;
        abort();
    }

    if(waitpid(mySlavePid, nullptr, 0) <= 0)
    {
        std::cerr << "Master's waitpid failed!" << std::endl;
        abort();
    };

    close(myReceiver);
    close(myTransmitter);

    munmap(mySharedMemory, 4*MAX_WIDTH*MAX_HEIGHT);
    mySharedMemory = nullptr;

    myRunning = false;
}

void Emulator::readScreen(cv::Mat4b& screen)
{
    const uint8_t cmd = CMD_READ_SCREEN;
    int rows = 0;
    int cols = 0;

    if( write(myTransmitter, &cmd, sizeof(uint8_t)) != 1 )
    {
        std::cout << "Pipe error" << std::endl;
        abort();
    }

    if( read(myReceiver, &rows, sizeof(int)) != sizeof(int) )
    {
        std::cout << "Pipe error" << std::endl;
        abort();
    }

    if( read(myReceiver, &cols, sizeof(int)) != sizeof(int) )
    {
        std::cout << "Pipe error" << std::endl;
        abort();
    }

    screen.create(rows, cols);
    memcpy(screen.ptr(), mySharedMemory, 4*rows*cols);

    for(cv::Vec4b& pix : screen)
    {
        pix[3] = 255;
    }
}

void Emulator::sendKeyDown(int key)
{
    const uint8_t cmd = CMD_EVENT;

    EmulatorEvent event;
    event.type = EMULATOR_EVENT_KEY_DOWN;
    event.key = key;

    write(myTransmitter, &cmd, sizeof(uint8_t));
    write(myTransmitter, &event, sizeof(event));
}

void Emulator::sendKeyUp(int key)
{
    const uint8_t cmd = CMD_EVENT;

    EmulatorEvent event;
    event.type = EMULATOR_EVENT_KEY_UP;
    event.key = key;

    write(myTransmitter, &cmd, sizeof(uint8_t));
    write(myTransmitter, &event, sizeof(event));
}

void emulator_read_event(struct EmulatorEvent* ev)
{
    std::lock_guard<std::mutex> lock(emulator_event_queue_mutex);

    if(emulator_event_queue.empty())
    {
        ev->type = EMULATOR_EVENT_NONE;
    }
    else
    {
        *ev = emulator_event_queue.front();
        emulator_event_queue.pop();
    }
}

void emulator_update_image(int w, int h, uint8_t* data)
{
    std::lock_guard<std::mutex> lock(emulator_screen_mutex);

    if( w > 0 && h > 0)
    {
        const cv::Size size(w,h);

        if(emulator_screen.size() != size)
        {
            emulator_screen.create(size);
        }

        memcpy(emulator_screen.ptr(), data, 4*w*h);

        /*
        if(internals->with_ui)
        {
            cv::imshow("im", internals->image);
            cv::waitKey(1);
        }
        */
    }
    else
    {
        emulator_screen = cv::Mat4b();
    }
}
