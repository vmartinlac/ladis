#include <thread>
#include <iostream>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "Agent.h"
#include "Checkers.h"

DamesAgent::DamesAgent()
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
}

void DamesAgent::extractCell(const cv::Mat4b& screen, int no, cv::Mat4b& cell)
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

void DamesAgent::typeText(LADIS::Interface* interface, const char* text)
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

void DamesAgent::play(LADIS::Interface* interface)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    typeText(interface, "CD DAME2020\nDA2020\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    typeText(interface, "\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    typeText(interface, "5\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    cv::Mat4b im;
    interface->getCurrentImage(im);

    Checkers::State s;
    readState(im, s);
    std::cout << s.getSquareGrid() << std::endl;
}

void DamesAgent::readState(const cv::Mat4b& screen, Checkers::State& s)
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
    s.setMyTurn(true);
}

void DamesAgent::computeFeatures(const cv::Mat4b& picture, cv::Vec6f& features)
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

char DamesAgent::predictCell(const cv::Vec6f& features)
{
    const char* values = " jJoO";

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

/*
empty [[ 4.44089210e-16 -1.11022302e-16  6.66133815e-16 -6.66133815e-16 -8.88178420e-16  4.44089210e-16]
homme joueur [ 1.44226667e+00  1.34842778e+00  5.83527000e-01  6.65310000e-01 3.18606667e+00  3.48021222e+00]
dame joueur [ 1.36443750e+00  1.50949500e+00  6.64148000e-01  8.31589000e-01 4.92916500e+00  5.33817500e+00]
homme adversaire [ 1.30302600e+00  1.40953400e+00  3.90906800e+00  4.22860400e+00 3.90906800e+00  4.22860400e+00]
dame adversaire [ 1.93052500e+00  2.21705500e+00  5.79157500e+00  6.65116000e+00 5.79157500e+00  6.65116000e+00]
*/
