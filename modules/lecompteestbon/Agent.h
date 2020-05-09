
#pragma once

#include <map>
#include <LADIS.h>

class LeCompteEstBonAgent : public LADIS::Agent
{
public:

    LeCompteEstBonAgent();

    void play(LADIS::Interface* interface) override;

protected:

    void typeText(LADIS::Interface* interface, const char* text);
    bool decodeStock(const cv::Mat4b& image, std::vector<int>& stock);
    bool decodeTarget(const cv::Mat4b& image, int& target);
    char extractCharacter(const std::vector<cv::Point>& points);

protected:

    struct Sample
    {
        Sample(float mx, float my, char _character)
        {
            moments[0] = mx;
            moments[1] = my;
            character = _character;
        }

        Sample(const cv::Vec2f& _moments, char _character)
        {
            moments = _moments;
            character = _character;
        }

        cv::Vec2f moments;
        char character;
    };

protected:

    std::map<char, std::tuple<int,bool> > myCharToKey;
    std::vector<Sample> mySamples;
};

