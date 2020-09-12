#include <thread>
#include <iostream>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <LADIS.h>
#include "Arena.h"
#include "Checkers.h"

std::unique_ptr<Arena> Arena::myInstance;

const std::vector<Checkers::State>& Arena::refLog() const
{
    return myLog;
}

void Arena::pressKey(int key)
{
    LADIS::Emulator::getInstance().keyDown(key);
    LADIS::Emulator::getInstance().keyUp(key);
}

Arena& Arena::getInstance()
{
    if(!myInstance)
    {
        myInstance.reset(new Arena());
    }

    return *myInstance;
}

void Arena::start()
{
    LADIS::Emulator& interface = LADIS::Emulator::getInstance();

    interface.start(true);

    // let dosbox initialize.
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // launch DAME2020 game.
    typeText("CD DAME2020\nDA2020\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // skip intro.
    typeText("\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void Arena::quit()
{
    LADIS::Emulator::getInstance().quit();
}

Arena::Arena()
{
    myMaxNumTurns = 200;
    myOpponentSkill = 0;
    mySaveScreens = true;
    myOutcome = OUTCOME_ERROR;

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
}

Arena::Outcome Arena::getOutcome()
{
    return myOutcome;
}

void Arena::extractCell(const cv::Mat4b& screen, int no, cv::Mat4b& cell)
{
    const cv::Rect checkerboard(0, 0, 441, 411);
    const int border = 1;
    const int SIDE = Checkers::SIDE;
    const int N = Checkers::N;

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

void Arena::typeText(const char* text)
{
    LADIS::Emulator& interface = LADIS::Emulator::getInstance();

    const int delay = 80;
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
            interface.keyDown(LADIS_KEY_LSHIFT);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        else if(!maj_required && maj)
        {
            maj = false;
            interface.keyUp(LADIS_KEY_LSHIFT);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        interface.keyDown(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        interface.keyUp(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    if(maj)
    {
        interface.keyUp(LADIS_KEY_LSHIFT);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void Arena::play(Agent* agent, Hook* hook)
{
    struct ActionStatePair
    {
        Checkers::Action action;
        Checkers::State state;
    };

    LADIS::Emulator& interface = LADIS::Emulator::getInstance();
    cv::Mat4b screen;
    Checkers::ActionIterator action_iterator;
    Checkers::State current_state;
    int num_consecutive_outcomes = 0;
    int num_turns = 0;
    bool go_on = true;

    myOutcome = OUTCOME_ERROR;
    myLog.clear();

    // select difficulty and start playing.
    {
        if( 0 <= myOpponentSkill && myOpponentSkill <= 5 )
        {
            pressKey(LADIS_KEY_0 + myOpponentSkill);
            if(myAgentStarts)
            {
                pressKey(LADIS_KEY_F2);
            }
            pressKey(LADIS_KEY_RETURN);

            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        else
        {
            throw std::runtime_error("internal error");
        }
    }

    if(!myAgentStarts)
    {
        Checkers::State s;
        s.setSquareGrid(
            " o o o o o\n"
            "o o o o o \n"
            " o o o o o\n"
            "o o o o o \n"
            " . . . . .\n"
            ". . . . . \n"
            " p p p p p\n"
            "p p p p p \n"
            " p p p p p\n"
            "p p p p p \n");
        s.setMyTurn(false);

        myLog.push_back(s);
    }

    while(go_on)
    {
        //if(hook) hook->getLogStream() << "* Getting current screen..." << std::endl;
        interface.getCurrentImage(screen);

        if(tryExtractOutcome(screen))
        {
            if(hook) hook->getLogStream() << "* Screen contains outcome!" << std::endl;
            num_consecutive_outcomes++;
            go_on = (num_consecutive_outcomes <= 2);
        }
        else
        {
            //if(hook) hook->getLogStream() << "* Screen does not contains outcome!" << std::endl;

            num_consecutive_outcomes = 0;

            //if(hook) hook->getLogStream() << "* Reading state..." << std::endl;
            readState(screen, current_state);

            if( current_state.isMyTurn() && (myLog.empty() || !(myLog.back() == current_state)) )
            {
                if(hook)
                {
                    hook->getLogStream()
                        << "* I have to play!" << std::endl
                        << "* Incoming state:" << std::endl
                        << current_state.getSquareGrid() << std::endl
                        << std::endl;
                }

                if(!myLog.empty())
                {
                    if(hook) hook->getLogStream() << "* Checking if last move was legal..." << std::endl;

                    if(myLog.back().isMyTurn())
                    {
                        throw std::runtime_error("internal error");
                    }

                    bool found = false;

                    {
                        action_iterator.init(myLog.back());

                        Checkers::Action action_;
                        Checkers::State state_;
                        while(!found && action_iterator.next(myLog.back(), action_, state_))
                        {
                            found = (current_state == state_);
                        }
                    }

                    if(found)
                    {
                        if(hook) hook->getLogStream() << "* Last move was legal!" << std::endl;
                    }
                    else
                    {
                        if(hook) hook->getLogStream() << "* Last move was not legal!" << std::endl;
                        go_on = false;
                        myOutcome = OUTCOME_ILLEGAL_OPPONENT_MOVE;
                    }
                }

                if(go_on)
                {
                    myLog.push_back(current_state);

                    // save current screen.
                    if(hook)
                    {
                        hook->receivedImage(screen);
                        hook->getLogStream() << "* Asking action from agent..." << std::endl;
                    }

                    Checkers::Action requested_action;
                    const bool ok = agent->getAction(current_state, requested_action);

                    if(ok)
                    {
                        bool found = false;
                        Checkers::State resulting_state;

                        if(hook) hook->getLogStream() << "* Checking legality of move given by agent..." << std::endl;

                        // Enumerate available actions.
                        {
                            action_iterator.init(current_state);

                            Checkers::Action action_;
                            Checkers::State state_;
                            while(!found && action_iterator.next(current_state, action_, state_))
                            {
                                if(requested_action == action_)
                                {
                                    resulting_state = state_;
                                    found = true;
                                }
                            }
                        }

                        if(found)
                        {
                            if(resulting_state.isMyTurn())
                            {
                                throw std::runtime_error("internal error");
                            }

                            if(hook) hook->getLogStream() << "* Move is legal." << std::endl;

                            myLog.push_back(resulting_state);

                            std::this_thread::sleep_for(std::chrono::milliseconds(1500));

                            if(hook) hook->getLogStream() << "* Move:";

                            for(int i=0; i<=requested_action.getNumMoves(); i++)
                            {
                                const int cell = (myAgentStarts) ? (Checkers::N-requested_action.getTrajectory(i)) : (requested_action.getTrajectory(i) + 1);

                                {
                                    std::stringstream ss;
                                    ss << cell << '\n';
                                    typeText(ss.str().c_str());
                                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                                }

                                if(hook) hook->getLogStream() << ' ' << cell;
                            }
                            if(hook) hook->getLogStream() << std::endl;

                            if(hook)
                            {
                                hook->getLogStream()
                                    << "Outcoming state:" << std::endl
                                    << resulting_state.getSquareGrid() << std::endl
                                    << std::endl;
                            }
                        }
                        else
                        {
                            if(hook) hook->getLogStream() << "* Illegal move from agent!" << std::endl;
                            myOutcome = OUTCOME_ILLEGAL_AGENT_MOVE;
                            go_on = false;
                        }
                    }
                    else
                    {
                        if(hook) hook->getLogStream() << "* Agent lost!" << std::endl;
                        myOutcome = OUTCOME_LOSE;
                        go_on = false;
                    }
                }

                if(go_on)
                {
                    num_turns++;
                    if(num_turns >= myMaxNumTurns)
                    {
                        myOutcome = OUTCOME_DRAW;
                        go_on = false;
                    }
                }
            }
            /*
            else
            {
                if(hook) hook->getLogStream() << "* I do not have to play!" << std::endl;
            }
            */
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Save last screen.
    interface.getCurrentImage(screen);
    if(hook) hook->receivedImage(screen);

    pressKey(LADIS_KEY_F10);
    pressKey(LADIS_KEY_ESCAPE);

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void Arena::readState(const cv::Mat4b& screen, Checkers::State& s)
{
    char grid[Checkers::N];

    for(int i=0; i<Checkers::N; i++)
    {
        cv::Mat4b ROI;
        extractCell(screen, i, ROI);

        cv::Vec6f features;
        computeFeatures(ROI, features);

        grid[i] = predictCell(features);
    }

    s.setFlatGrid(grid);
    s.setMyTurn(extractIsMyTurn(screen));
}

bool Arena::extractIsMyTurn(const cv::Mat4b& screen)
{
    const uint8_t ref[49] =
    {
        0, 255, 255, 255, 255, 0, 0,
        255, 255, 0, 0, 255, 255, 0,
        0, 0, 0, 0, 255, 255, 0,
        0, 0, 0, 255, 255, 0, 0,
        0, 0, 255, 255, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 255, 255, 0, 0, 0
    };

    const cv::Rect location(0, 461, 7, 7);
    cv::Mat4b ROI = screen(location);

    int dist = 0;
    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            dist += std::abs(ROI(i,j)[0] - ref[i*7+j]);
            dist += std::abs(ROI(i,j)[1] - ref[i*7+j]);
            dist += std::abs(ROI(i,j)[2] - ref[i*7+j]);
        }
    }

    return (dist == 0);
}

void Arena::computeFeatures(const cv::Mat4b& picture, cv::Vec6f& features)
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

char Arena::predictCell(const cv::Vec6f& features)
{
    const char* values = (myAgentStarts) ? ".oOpP" : ".pPoO";

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

void Arena::setMaxNumTurns(int value)
{
    myMaxNumTurns = value;
}

void Arena::setAgentStarts(bool value)
{
    myAgentStarts = value;
}

void Arena::setSaveScreens(bool value)
{
    mySaveScreens = value;
}

void Arena::setOpponentSkill(int skill)
{
    myOpponentSkill = skill;
}

bool Arena::tryExtractOutcome(const cv::Mat4b& screen)
{
    static uint8_t image_win[] =
    {
          0, 255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255, 255, 255, 255, 255, 255,   0,
        255, 255,   0,   0,   0, 255, 255,   0,   0, 255, 255,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255, 255,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0,
        255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255,   0, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0,
        255, 255,   0,   0, 255, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0, 255, 255, 255,   0, 255, 255,   0, 255, 255, 255, 255,   0, 255, 255, 255, 255, 255, 255,   0,   0,
        255, 255,   0,   0,   0, 255, 255,   0, 255, 255, 255, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0, 255, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0,
        255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0,
          0, 255, 255, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0,   0, 255, 255, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255, 255, 255, 255, 255, 255,   0
    };

    static uint8_t image_lose[] =
    {
        255, 255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255, 255, 255,   0, 255, 255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255,   0,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0,
        255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0,
        255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0,
        255, 255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0,
        255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0,
        255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0, 255, 255,   0,
        255, 255,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0
    };

    cv::Mat4b ROI_win = screen(cv::Rect(528, 219, 40, 7));
    cv::Mat1b ref_win(7, 40, image_win);

    cv::Mat4b ROI_lose = screen(cv::Rect(528, 219, 40, 7));
    cv::Mat1b ref_lose(7, 40, image_lose);

    bool ret = false;

    if( compareGrayscale(ROI_win, ref_win) )
    {
        ret = true;
        myOutcome = OUTCOME_WIN;
    }
    else if( compareGrayscale(ROI_lose, ref_lose) )
    {
        ret = true;
        myOutcome = OUTCOME_LOSE;
    }

    return ret;
}

bool Arena::compareGrayscale(const cv::Mat4b& m0, const cv::Mat1b& m1)
{
    if(m0.size() != m1.size())
    {
        throw std::runtime_error("internal error");
    }

    int dist = 0;

    for(int i=0; i<m0.rows; i++)
    {
        for(int j=0; j<m0.cols; j++)
        {
            dist += std::abs( m0(i,j)[0] - m1(i,j) );
            dist += std::abs( m0(i,j)[1] - m1(i,j) );
            dist += std::abs( m0(i,j)[2] - m1(i,j) );
        }
    }

    return dist == 0;
}

