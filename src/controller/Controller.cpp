#include <iostream>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include "Controller.h"

Controller::Controller()
{
    for(char i=0; i<25; i++)
    {
        myCharToKey['a'+i] = std::make_tuple(EMULATOR_KEY_a+i, false);
        myCharToKey['A'+i] = std::make_tuple(EMULATOR_KEY_a+i, true);
    }

    for(char i=0; i<10; i++)
    {
        myCharToKey['0'+i] = std::make_tuple(EMULATOR_KEY_0 + i, false);
    }

    myCharToKey[' '] = std::make_tuple(EMULATOR_KEY_SPACE, false);
    myCharToKey['\n'] = std::make_tuple(EMULATOR_KEY_RETURN, false);

    myCharToKey[':'] = std::make_tuple(EMULATOR_KEY_SEMICOLON, true);

    myCharToKey['+'] = std::make_tuple(EMULATOR_KEY_KP_PLUS, false);
    myCharToKey['-'] = std::make_tuple(EMULATOR_KEY_KP_MINUS, false);
    myCharToKey['*'] = std::make_tuple(EMULATOR_KEY_KP_MULTIPLY, false);
    myCharToKey['/'] = std::make_tuple(EMULATOR_KEY_KP_DIVIDE, false);

    myFont[27975301350782819] = "M";
    myFont[17560727076430654] = "O";
    myFont[8782226240254751] = "D";
    myFont[35750634519200639] = "E";
    myFont[0] = " ";
    myFont[3390893860850688] = ":";
    myFont[17560727025901688] = "J";
    myFont[17560727076430691] = "U";
    myFont[3390945601784895] = "T";
    myFont[35575241497247744] = "a";
    myFont[232893482304208896] = "p";
    myFont[35469692726280192] = "e";
    myFont[1695472942448640] = "r";
    myFont[847737407079295] = "F";
    myFont[17746169414028812] = "1";
    myFont[17560727073980416] = "o";
    myFont[35575125585887232] = "u";
    myFont[16914990365153308] = "l";
    myFont[27975336041054208] = "m";
    myFont[31089118146355296] = "d";
    myFont[16720700190425859] = "b";
    myFont[8447819276746759] = "è";
    myFont[8500531025687326] = "3";
    myFont[27925517493796864] = "x";
    myFont[35469160150335488] = "c";
    myFont[16914990365409304] = "i";
    myFont[17838742991405056] = "s";
    myFont[27975421473797660] = "A";
    myFont[17789024868512542] = "2";
    myFont[17560727076430654] = "0";
    myFont[6768619451452440] = "(";
    myFont[1087645801772744728] = "j";
    myFont[27975301212340224] = "n";
    myFont[1702147483241478] = ")";
    myFont[17560313148957502] = "C";
    myFont[27975370337642339] = "N";
    myFont[27975301212340995] = "h";
    myFont[4494713869985251328] = "g";
    myFont[27975300610745151] = "R";
    myFont[8500427675284254] = "S";
    myFont[847737413395263] = "P";
    myFont[17842201449161535] = "B";
    myFont[473314844] = "°";
    myFont[7941099844993024] = "v";
    myFont[35522189253804039] = "à";
    myFont[8500531360105279] = "5";
    myFont[8457495182576670] = "I";
    myFont[7941099845018467] = "V";
}

std::string Controller::extractString(const cv::Mat4b& image, const cv::Vec4b& foreground, const cv::Vec4b& background)
{
    bool ok = true;
    std::string ret;

    if(ok)
    {
        ok = image.rows == 8 && (image.cols % 8) == 0;
    }

    if(ok)
    {
        const int N = image.cols / 8;

        for(int n=0; ok && n<N; n++)
        {
            cv::Mat4b ROI = image(cv::Rect(8*n, 0, 8, 8));

            uint64_t compiled = 0;
            int index = 0;

            for(int i=0; ok && i<8; i++)
            {
                for(int j=0; ok && j<8; j++)
                {
                    const cv::Vec4b& pixel = ROI(i,j);

                    if(pixel == foreground)
                    {
                        compiled |= (uint64_t(1) << index);
                    }
                    else if(pixel != background)
                    {
                        ok = false;
                    }

                    index++;
                }
            }

            if(ok)
            {
                std::map<uint64_t, std::string>::iterator it = myFont.find(compiled);

                if(it == myFont.end())
                {
                    ok = false;
                }
                else
                {
                    ret += it->second;
                }
            }
        }
    }

    if(!ok)
    {
        ret = "";
    }

    return ret;
}

void Controller::run(Emulator* emulator, Agent* agent, bool agent_plays_first, int difficulty)
{
    myEmulator = emulator;
    myAgent = agent;
    myAgentPlaysFirst = agent_plays_first;
    myDifficulty = difficulty;

    cv::Mat4b screen;
    int result = Agent::RESULT_ERROR;
    bool go_on = true;

    agent->beginMatch();
    emulator->start();

    if(go_on)
    {
        go_on = waitFirstFrame(screen);
    }

    if(go_on)
    {
        typeText("mount c /DATA/victor/emulation/dosbox\n");
        typeText("c:\n");
        typeText("cd DAME2020\n");
        typeText("DA2020\n");

        go_on = waitForIntroScreen();
    }

    if(go_on)
    {
        typeText("\n");

        go_on = waitForMenu();
    }

    if(go_on)
    {
        go_on = processMenu();
    }

    emulator->stop();
    agent->endMatch(result);
}

bool Controller::processMenu()
{
    // TODO

    return false;
}

bool Controller::waitFirstFrame(cv::Mat4b& screen)
{
    const ClockType::time_point t0 = ClockType::now();

    myEmulator->readScreen(screen);

    while( std::chrono::duration_cast<std::chrono::milliseconds>(ClockType::now() - t0).count() < 1000 && screen.size() == cv::Size(0,0))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        myEmulator->readScreen(screen);
    }

    return screen.size() != cv::Size(0, 0);
}

bool Controller::checkIntroScreen()
{
    const cv::Vec4b refcolor(0xaa, 0xaa, 0x00, 0xFF);
    cv::Mat4b screen;

    bool ret = false;

    myEmulator->readScreen(screen);

    if(screen.size() == cv::Size(640, 480))
    {
        const int margin = 10;

        std::array<cv::Mat4b,2> ROI;
        ROI[0] = screen(cv::Rect(0, 0, margin, 480));
        ROI[1] = screen(cv::Rect(640-margin, 0, margin, 480));

        ret = true;
        for(cv::Mat4b& item : ROI)
        {
            ret = ret && std::all_of(item.begin(), item.end(), [refcolor] (const cv::Vec4b& x) { return x == refcolor; });
        }
    }

    return ret;
}

bool Controller::waitForIntroScreen()
{
    const ClockType::time_point t0 = ClockType::now();

    bool ret = checkIntroScreen();

    while( !ret && std::chrono::duration_cast<std::chrono::milliseconds>(ClockType::now() - t0).count() < 2000 )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ret = checkIntroScreen();
    }

    return ret;
}

bool Controller::waitForMenu()
{
    const ClockType::time_point t0 = ClockType::now();
    cv::Mat4b screen;
    bool go_on = true;

    while(go_on && std::chrono::duration_cast<std::chrono::milliseconds>(ClockType::now() - t0).count() < 2000 )
    {
        myEmulator->readScreen(screen);

        const bool b0 = extractString(screen(cv::Rect(442, 20, 48, 8))) == "NIVEAU";
        const bool b1 = extractString(screen(cv::Rect(442, 143, 32, 8))) == "M0DE";
        const bool b2 = extractString(screen(cv::Rect(442, 369, 128, 8))) == "ENTREE 0U ESPACE";
        const bool b3 = extractString(screen(cv::Rect(442, 389, 96, 8))) == "P0UR DEBUTER";

        if(b0 && b1 && b2 && b3)
        {
            go_on = false;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    return !go_on;
}

void Controller::typeText(const char* text)
{
    const int delay = 10;
    bool maj = false;

    for(const char* p=text; *p != 0; p++)
    {
        if( myCharToKey.find(*p) == myCharToKey.end() )
        {
            throw std::runtime_error("Character not found!");
        }

        const int key = std::get<0>(myCharToKey[*p]);
        const bool maj_required = std::get<1>(myCharToKey[*p]);

        if(maj_required && !maj)
        {
            maj = true;
            myEmulator->sendKeyDown(EMULATOR_KEY_LSHIFT);
            if(delay > 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }
        else if(!maj_required && maj)
        {
            maj = false;
            myEmulator->sendKeyUp(EMULATOR_KEY_LSHIFT);
            if(delay > 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }

        myEmulator->sendKeyDown(key);

        if(delay > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        myEmulator->sendKeyUp(key);

        if(delay > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    if(maj)
    {
        myEmulator->sendKeyUp(EMULATOR_KEY_LSHIFT);

        if(delay > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }
}

