#include <iostream>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include "Checkers.h"
#include "Controller.h"
#include "Utils.h"

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
    myFont[35750646549852287] = "Z";
    myFont[33830320083450936] = "4";
    myFont[35750633512567555] = "L";
    myFont[27975301684487011] = "H";
    myFont[3390893860061184] = ".";
    myFont[3377751463435788] = "!";
    myFont[35575125848056638] = "G";
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

    if(difficulty < 0 || difficulty > 5)
    {
        std::cerr << "Incorrect value for difficulty!" << std::endl;
        abort();
    }

    int result = Agent::RESULT_CONTROLLER_ERROR;
    bool go_on = true;

    int mode = MODE_WAIT_MY_TURN;
    bool need_new_screen = true;
    Checkers::Grid current_grid;
    Checkers::Action current_action;
    int trajectory_index = -1;
    cv::Mat4b screen;

    agent->beginMatch(myAgentPlaysFirst, myDifficulty);
    emulator->start();

    if(go_on)
    {
        go_on = waitFirstFrame();
    }

    if(go_on)
    {
        std::default_random_engine RNG;
        Utils::seedRNG(RNG);

        //const double mean_time = 15.0;
        //std::exponential_distribution<double> X(1.0/mean_time);

        const double max_time = 45.0;
        std::uniform_real_distribution<double> X(0.0, max_time);

        const double actual_time = X(RNG);

        //std::cout << "Sleeping for " << actual_time << " seconds before starting playing." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(actual_time * 1.0e3)));
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

    while(go_on)
    {
        if(need_new_screen)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(6000));
            myEmulator->readScreen(screen);

            //
            {
                static int image_id = 0;
                std::stringstream filename;
                filename << "screen_" << image_id << ".png";
                cv::imwrite(filename.str(), screen);
                image_id++;
            }
            //
        }

        need_new_screen = true;

        if(mode == MODE_WAIT_MY_TURN)
        {
            if( extractString(screen(cv::Rect(448, 219, 136, 8))) == "V0US AVEZ GAGNE !" )
            {
                go_on = false;
                result = Agent::RESULT_AGENT_WON;
            }
            else if( extractString(screen(cv::Rect(448, 219, 136, 8))) == "V0US AVEZ PERDU !" )
            {
                go_on = false;
                result = Agent::RESULT_AGENT_LOST;
            }
            else if( extractString(screen(cv::Rect(444, 246, 56, 8))) == "JE J0UE" )
            {
            }
            else if( extractString(screen(cv::Rect(445, 246, 120, 8))) == "JE REFLECHIS..." )
            {
            }
            else if( extractString(screen(cv::Rect(0, 445, 120, 8))) == "A V0US DE J0UER" )
            {
                extractGrid(screen, current_grid);

                Checkers::State current_state;
                current_state.setGrid(current_grid);
                current_state.setMyTurn(true);

                const bool agent_ok = myAgent->play(current_state, current_action);

                if(agent_ok)
                {
                    if(current_action.getNumMoves() < 1)
                    {
                        go_on = false;
                        result = Agent::RESULT_AGENT_ILLEGAL_MOVE;
                    }
                    else
                    {
                        trajectory_index = 0;
                        mode = MODE_TYPE_TRAJECTORY;
                        need_new_screen = false;
                    }
                }
                else
                {
                    go_on = false;
                    result = Agent::RESULT_AGENT_GAVE_UP;
                }
            }
        }
        else if(mode == MODE_TYPE_TRAJECTORY)
        {
            if( extractString(screen(cv::Rect(448, 219, 136, 8))) == "V0US AVEZ GAGNE !" )
            {
                go_on = false;
                result = Agent::RESULT_AGENT_WON;
            }
            else if( extractString(screen(cv::Rect(448, 219, 136, 8))) == "V0US AVEZ PERDU !" )
            {
                go_on = false;
                result = Agent::RESULT_AGENT_LOST;
            }
            else if( extractString(screen(cv::Rect(0, 430, 104, 8))) == "C0UP INTERDIT" )
            {
                go_on = false;
                result = Agent::RESULT_AGENT_ILLEGAL_MOVE;
            }
            else if( extractString(screen(cv::Rect(444, 246, 56, 8))) == "JE J0UE" )
            {
                mode = MODE_WAIT_MY_TURN;
            }
            else if( extractString(screen(cv::Rect(445, 246, 120, 8))) == "JE REFLECHIS..." )
            {
                mode = MODE_WAIT_MY_TURN;
            }
            else if( trajectory_index >= current_action.getNumCells() )
            {
                go_on = false;
                result = Agent::RESULT_CONTROLLER_ERROR;
            }
            else
            {
                Checkers::Grid this_grid;
                extractGrid(screen, this_grid);

                if(current_grid == this_grid)
                {
                    std::stringstream text;

                    const int cell = current_action.getCell(trajectory_index);

                    trajectory_index++;

                    if(myAgentPlaysFirst)
                    {
                        text << (Checkers::N-cell) << '\n';
                    }
                    else
                    {
                        text << (cell+1) << '\n';
                    }

                    typeText(text.str().c_str());

                    if( trajectory_index >= current_action.getNumCells() )
                    {
                        mode = MODE_WAIT_MY_TURN;
                    }
                }
                else
                {
                    mode = MODE_WAIT_MY_TURN;
                    need_new_screen = false;
                }
            }
        }
    }

    emulator->stop();
    agent->endMatch(result);
}

void Controller::extractGrid(const cv::Mat4b& screen, Checkers::Grid& grid)
{
    for(int i=0; i<Checkers::N; i++)
    {
        cv::Mat4b ROI;
        extractCell(screen, i, ROI);

        cv::Vec6f features;
        computeFeatures(ROI, features);

        grid[i] = predictCell(features);
    }
}

void Controller::extractCell(const cv::Mat4b& screen, int no, cv::Mat4b& cell)
{
    const cv::Rect checkerboard(0, 0, 441, 411);
    constexpr int border = 1;
    constexpr int SIDE = Checkers::SIDE;
    constexpr int N = Checkers::N;

    const int i = no;
    const int y = SIDE-1-(2*i+1)/SIDE;
    const int x = SIDE-1-2*(i%(SIDE/2)) - (y%2);

    const cv::Rect ROI(
        checkerboard.x + border + x*(checkerboard.width-border)/SIDE,
        checkerboard.y + border + y*(checkerboard.height-border)/SIDE,
        (checkerboard.width-border)/SIDE-border,
        (checkerboard.height-border)/SIDE-border);

    cell = screen(ROI);
}

bool Controller::processMenu()
{
    cv::Mat4b screen;
    bool go_on = true;
    bool ret = false;

    while(go_on)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        myEmulator->readScreen(screen);

        if(screen.size() != cv::Size(640, 480))
        {
            std::cout << "Incorrect image size!" << std::endl;
            go_on = false;
            ret = false;
        }
        else
        {
            ret = true;

            if(ret)
            {
                const std::string s2 = extractString(screen(cv::Rect(498, 143, 24, 8))); // game mode

                if(s2 != "JEU")
                {
                    ret = false;
                    //typeKeys({EMULATOR_KEY_F1});
                    go_on = false;
                    std::cout << "Not game mode!" << std::endl;
                }
            }

            if(ret)
            {
                const std::string s0 = extractString(screen(cv::Rect(442, 266, 184, 8))); // first player

                if(s0 == "DA2020 (jaune) C0MMENCE")
                {
                    if(myAgentPlaysFirst)
                    {
                        ret = false;
                        typeKeys({EMULATOR_KEY_F2});
                        //std::cout << "Setting first player to agent." << std::endl;
                    }
                }
                else if(s0 == "V0US (jaune) C0MMENCEZ ")
                {
                    if(!myAgentPlaysFirst)
                    {
                        ret = false;
                        typeKeys({EMULATOR_KEY_F2});
                        //std::cout << "Setting first player to DA2020." << std::endl;
                    }
                }
                else
                {
                    go_on = false;
                    ret = false;
                    std::cout << "Could not extract first player text from image." << std::endl;
                }
            }

            if(ret)
            {
                const std::string s1 = extractString(screen(cv::Rect(528, 20, 8, 8))); // difficulty

                if(s1 != std::to_string(myDifficulty))
                {
                    ret = false;
                    typeKeys({EMULATOR_KEY_0 + myDifficulty});
                    //std::cout << "Setting difficulty to " << myDifficulty << "." << std::endl;
                }
            }

            if(ret)
            {
                go_on = false;
                typeKeys({EMULATOR_KEY_RETURN});
                //std::cout << "Ready to play" << std::endl;
            }
        }
    }

    return ret;
}

bool Controller::waitFirstFrame()
{
    cv::Mat4b screen;

    const ClockType::time_point t0 = ClockType::now();

    myEmulator->readScreen(screen);

    while( std::chrono::duration_cast<std::chrono::milliseconds>(ClockType::now() - t0).count() < 1000 && screen.size() == cv::Size(0,0) )
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

void Controller::typeKeys(std::initializer_list<int> keys)
{
    const int delay = 10;

    for(int key : keys)
    {
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

void Controller::computeFeatures(const cv::Mat4b& picture, cv::Vec6f& features)
{
    const int margin = 4;

    const cv::Mat4b ROI = picture(cv::Rect(margin, margin, picture.cols-2*margin, picture.rows-2*margin));

    std::fill(features.val, features.val+6, 0.0f);

    for(int i=0; i<ROI.rows; i++)
    {
        for(int j=0; j<ROI.cols; j++)
        {
            const cv::Vec4b& pix = ROI(i,j);

            const float fi = float(i) / float(ROI.rows-1);
            const float fj = float(j) / float(ROI.cols-1);

            features[0] += pix[0] * fi;
            features[1] += pix[0] * fj;
            features[2] += pix[1] * fi;
            features[3] += pix[1] * fj;
            features[4] += pix[2] * fi;
            features[5] += pix[2] * fj;
        }
    }

    features /= (255.0 * ROI.rows*ROI.cols);
}

char Controller::predictCell(const cv::Vec6f& features)
{
    const char* values = (myAgentPlaysFirst) ? ".oOpP" : ".pPoO";

    /*
    empty [[ 4.44089210e-16 -1.11022302e-16  6.66133815e-16 -6.66133815e-16 -8.88178420e-16  4.44089210e-16]
    homme joueur [ 1.44226667e+00  1.34842778e+00  5.83527000e-01  6.65310000e-01 3.18606667e+00  3.48021222e+00]
    dame joueur [ 1.36443750e+00  1.50949500e+00  6.64148000e-01  8.31589000e-01 4.92916500e+00  5.33817500e+00]
    homme adversaire [ 1.30302600e+00  1.40953400e+00  3.90906800e+00  4.22860400e+00 3.90906800e+00  4.22860400e+00]
    dame adversaire [ 1.93052500e+00  2.21705500e+00  5.79157500e+00  6.65116000e+00 5.79157500e+00  6.65116000e+00]
    */

    static const float reference_features[6*5] =
    {
        0.00000000e+00, 1.73472348e-17, 6.93889390e-18, 0.00000000e+00, -2.77555756e-17, -2.77555756e-17, // empty
        0.05926673, 0.04979867, 0.0228783, 0.0245536, 0.12645144, 0.12858,  // dame adverse
        0.0543731, 0.05275735, 0.0257872, 0.0277048, 0.19730725, 0.1882615, // homme adverse
        0.0518549, 0.05209036, 0.1555648, 0.156271,  0.1555648, 0.156271, // homme joueur
        0.0760706, 0.0797269, 0.228212, 0.2391805, 0.228212, 0.2391805 // dame joueur
    };

    int k = 0;
    double best_dist = cv::norm(cv::Vec6f(reference_features), features);

    for(int i=1; i<5; i++)
    {
        const double dist = cv::norm(cv::Vec6f(reference_features+6*i), features);

        if(dist < best_dist)
        {
            best_dist = dist;
            k = i;
        }
    }

    return values[k];
}
