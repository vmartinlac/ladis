#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>
#include <opencv2/highgui.hpp>
#include "LADIS_glue.h"
#include "LADIS.h"

int DOSBOX_main();

namespace LADIS
{
    struct Internals
    {
        Internals()
        {
            agent = nullptr;
            with_ui = false;
        }

        bool with_ui;
        std::mutex image_mutex;
        cv::Mat4b image;
        std::mutex events_mutex;
        std::queue<LADIS_Event> event_queue;

        std::thread thread_dosbox;

        Agent* agent;
        std::thread thread_ai;
    };

    static std::unique_ptr<Internals> internals;

    //

    class EmulatorImpl : public Emulator
    {
    public:

        void pushEvent(const LADIS_Event& ev)
        {
            internals->events_mutex.lock();
            internals->event_queue.push(ev);
            internals->events_mutex.unlock();
        }

        void start(bool with_ui) override
        {
            auto proc = [] ()
            {
                DOSBOX_main();
            };

            if(internals)
            {
                throw std::runtime_error("already initialized");
            }

            internals.reset(new Internals());
            internals->with_ui = with_ui;
            internals->thread_dosbox = std::thread(proc);
        }

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

    Emulator& Emulator::getInstance()
    {
        static std::unique_ptr<EmulatorImpl> emulator;

        if(!emulator)
        {
            emulator.reset(new EmulatorImpl());
        }

        return *emulator;
    }

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

        DOSBOX_main();

        internals->thread_ai.join();
        internals.reset();

        /*
        if(with_ui)
        {
            cv::destroyAllWindows();
        }
        */
    }
}

void LADIS_read_event(struct LADIS_Event* ev)
{
    using namespace LADIS;

    internals->events_mutex.lock();

    if( internals->event_queue.empty() )
    {
        ev->type = LADIS_EVENT_NONE;
    }
    else
    {
        *ev = internals->event_queue.front();
        internals->event_queue.pop();
    }

    internals->events_mutex.unlock();
}

void LADIS_update_image(int w, int h, uint8_t* data)
{
    using namespace LADIS;

    internals->image_mutex.lock();

    if( w > 0 && h > 0)
    {
        const cv::Size size(w,h);

        if(internals->image.size() != size)
        {
            internals->image.create(size);
        }

        std::copy(data, data+4*w*h, internals->image.ptr());

        if(internals->with_ui)
        {
            cv::imshow("im", internals->image);
            cv::waitKey(1);
        }
    }
    else
    {
        internals->image = cv::Mat4b();
    }

    internals->image_mutex.unlock();
}

