#include <iostream>
#include <queue>
#include <opencv2/core.hpp>
#include "EmulatorInternals.h"

/*
class EmulatorInternals
{
public:

    static void pushEvent(const EmulatorEvent& ev)
    {
        std::lock_guard<std::mutex> lock(myEventsMutex);
        myEventQueue.push(ev);
    }

    static void popEvent(EmulatorEvent& ev)
    {
        std::lock_guard<std::mutex> lock(myEventsMutex);

        if(myEventQueue.empty())
        {
            ev.type = EMULATOR_EVENT_NONE;
        }
        else
        {
            ev = myEventQueue.front();
            myEventQueue.pop();
        }
    }

    static void setScreen(const cv::Mat4b& screen);

    static void getScreen(cv::Mat4b& screen);

protected:

    static std::mutex myScreenMutex;
    static cv::Mat4b myScreen;
    static std::mutex myEventsMutex;
    static std::queue<EmulatorEvent> myEventQueue;
};
*/

std::mutex emulator_screen_mutex;
cv::Mat4b emulator_screen;
std::mutex emulator_event_queue_mutex;
std::queue<EmulatorEvent> emulator_event_queue;


/*
void quit() override
{
    if(!internals)
    {
        throw std::runtime_error("not initialized");
    }

    LADIS_Event ev;
    ev.type = LADIS_EVENT_QUIT;
    pushEvent(ev);

    internals->thread_dosbox.join();
    internals.reset();
}

void getCurrentImage(cv::Mat4b& image) override
{
    internals->image_mutex.lock();
    image = internals->image.clone();
    internals->image_mutex.unlock();

    for(cv::Vec4b& pix : image)
    {
        pix[3] = 255;
    }
}

void mouseMove(int dx, int dy) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_MOUSE_MOVED;
    ev.dx = dx;
    ev.dy = dy;

    pushEvent(ev);
}

void mouseUp(int button) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_MOUSE_BUTTON_UP;
    ev.button = button;

    pushEvent(ev);
}

void mouseDown(int button) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_MOUSE_BUTTON_DOWN;
    ev.button = button;

    pushEvent(ev);
}

void keyUp(int key) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_KEY_UP;
    ev.key = key;

    pushEvent(ev);
}

void keyDown(int key) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_KEY_DOWN;
    ev.key = key;

    pushEvent(ev);
}

};

///////

class InterfaceImpl : public Interface
{
public:

void getCurrentImage(cv::Mat4b& image) override
{
    internals->image_mutex.lock();
    image = internals->image.clone();
    internals->image_mutex.unlock();

    for(cv::Vec4b& pix : image)
    {
        pix[3] = 255;
    }
}

void mouseMove(int dx, int dy) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_MOUSE_MOVED;
    ev.dx = dx;
    ev.dy = dy;

    pushEvent(ev);
}

void mouseUp(int button) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_MOUSE_BUTTON_UP;
    ev.button = button;

    pushEvent(ev);
}

void mouseDown(int button) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_MOUSE_BUTTON_DOWN;
    ev.button = button;

    pushEvent(ev);
}

void keyUp(int key) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_KEY_UP;
    ev.key = key;

    pushEvent(ev);
}

void keyDown(int key) override
{
    LADIS_Event ev;

    ev.type = LADIS_EVENT_KEY_DOWN;
    ev.key = key;

    pushEvent(ev);
}

void quit()
{
    LADIS_Event ev;
    ev.type = LADIS_EVENT_QUIT;
    pushEvent(ev);
}

void pushEvent(const LADIS_Event& ev)
{
    internals->events_mutex.lock();
    internals->event_queue.push(ev);
    internals->events_mutex.unlock();
}
};

static void entry_point_ai()
{
InterfaceImpl interface;

if(internals->agent == nullptr)
{
    std::cout << "There is no agent!" << std::endl;
}
else
{
    internals->agent->play(&interface);
}

interface.quit();
}

void run(Agent* agent, bool with_ui)
{
internals.reset(new Internals());
internals->agent = agent;
internals->with_ui = with_ui;
internals->thread_ai = std::thread(entry_point_ai);

std::cout << "hello" << std::endl;

const char* config_string =
    "[render]\n"
    "scaler=none\n"
    "[autoexec]\n"
    "#keyb fr\n"
    "mount c: /DATA/victor/emulation/dosbox\n"
    "c:\n";
DOSBOX_main(config_string);


internals->thread_ai.join();
internals.reset();

}
}
*/

void emulator_read_event(struct EmulatorEvent* ev)
{
    //EmulatorInternals::popEvent(*ev);
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

        std::copy(data, data+4*w*h, emulator_screen.ptr());

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

