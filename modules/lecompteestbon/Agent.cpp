#include <thread>
#include <iostream>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "Agent.h"
#include "Solver.h"
#include "Segmentation.h"

//
#include <fstream>
//

LeCompteEstBonAgent::LeCompteEstBonAgent()
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

    // initialize samples table.

    mySamples.emplace_back( 3128, 4968, '0');
    mySamples.emplace_back( 1126, 2366, '1');
    mySamples.emplace_back( 2584, 4064, '2');
    mySamples.emplace_back( 2614, 4166, '2');
    mySamples.emplace_back( 3080, 4536, '3');
    mySamples.emplace_back( 3116, 4644, '3');
    mySamples.emplace_back( 2774, 3702, '4');
    mySamples.emplace_back( 2620, 4588, '5');
    mySamples.emplace_back( 2722, 4378, '5');
    mySamples.emplace_back( 2752, 4600, '5');
    mySamples.emplace_back( 3092, 5116, '6');
    mySamples.emplace_back( 2112, 2736, '7');
    mySamples.emplace_back( 3196, 5076, '8');
    mySamples.emplace_back( 3300, 5036, '9');
    mySamples.emplace_back( 90, 90, '.');
}

void LeCompteEstBonAgent::typeText(LADIS::Interface* interface, const char* text)
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

void LeCompteEstBonAgent::play(LADIS::Interface* interface)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    typeText(interface, "CD CHIFFRES\nCL\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    typeText(interface, "c\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    typeText(interface, "j\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    typeText(interface, "1\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    typeText(interface, "e\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    typeText(interface, "ai\n");

    for(int iter=0; iter<10; iter++)
    {
        std::cout << "New game!" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        cv::Mat4b img;
        interface->getCurrentImage(img);
        cv::imwrite("00_problem.png", img);

        std::vector<int> stock;
        int target = 0;

        const bool has_stock = decodeStock(img, stock);
        const bool has_target = decodeTarget(img, target);

        if(!has_stock || !has_target)
        {
            std::cout << "Could not read stock or target!" << std::endl;
            exit(1);
        }

        {
            std::cout << "Stock = {";
            for(int number : stock)
            {
                std::cout << ' ' << number;
            }
            std::cout << " }" << std::endl;
            std::cout << "Target = " << target << std::endl;
        }

        std::vector< std::vector<Solver::Operation> > solutions;
        Solver solver;
        solver.solve(stock, target, std::chrono::milliseconds(30000), 1, solutions);

        if(solutions.empty())
        {
            std::cout << "Failed to compute a solution!" << std::endl;
            exit(1);
        }

        {
            cv::Mat4b other_image;
            bool go_on = true;

            while(go_on)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                interface->getCurrentImage(other_image);
                go_on = ( img.size() == other_image.size() && std::equal(img.begin(), img.end(), other_image.begin()) );
            }
        }

        std::cout << "Waking up!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        const bool my_turn = (iter % 2 == 0);
        if(my_turn)
        {
            {
                std::stringstream ss;
                
                ss << std::get<3>(solutions.front().back()) << "\n";
                typeText(interface, ss.str().c_str());
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            for( const Solver::Operation& op : solutions.front() )
            {
                std::stringstream ss;
                
                ss << std::get<1>(op) << ' ' << std::get<0>(op) << ' ' << std::get<2>(op) << '\n';
                typeText(interface, ss.str().c_str());
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(14000));
        }

        typeText(interface, "\n");
    }
}

bool LeCompteEstBonAgent::decodeStock(const cv::Mat4b& image, std::vector<int>& stock)
{
    bool ret = true;

    cv::Mat4b ROI = image(cv::Rect(90, 110, 448, 40));
    //cv::imwrite("sub.png", ROI);

    std::vector<Segmentation::Component> components;

    Segmentation seg;
    seg.segment(ROI, components);

    int root = -1;
    int i = 0;
    for(Segmentation::Component& c : components)
    {
        if(c.neighbors.size() == 6 && c.gray_level == 0)
        {
            if(root == -1)
            {
                root = i;
            }
            else
            {
                ret = false;
            }
        }
        i++;
    }

    ret = ret && (root >= 0);

    stock.clear();

    if(ret)
    {
        for(int plate : components[root].neighbors)
        {
            std::vector<int> components_for_digits;

            std::copy_if(
                components[plate].neighbors.begin(),
                components[plate].neighbors.end(),
                std::back_inserter(components_for_digits),
                [root] (int item) { return (item != root); } );

            auto comp = [&components] (int a, int b)
            {
                return components[a].pixels.front().x > components[b].pixels.front().x;
            };

            std::sort(
                components_for_digits.begin(),
                components_for_digits.end(),
                comp);

            int plate_number = 0;
            int base = 1;

            for(int digit_comp : components_for_digits)
            {
                if(ret)
                {
                    const char character = extractCharacter(components[digit_comp].pixels);

                    if( '0' <= character && character <= '9' )
                    {
                        plate_number += base * int(character - '0');
                        base *= 10;
                    }
                    else if(character != '.')
                    {
                        ret = false;
                    }
                }
            }

            if(ret)
            {
                stock.push_back(plate_number);
            }
        }
    }

    return ret;
}

bool LeCompteEstBonAgent::decodeTarget(const cv::Mat4b& image, int& target)
{
    bool ret = true;

    cv::Rect areas[3];
    areas[0] = cv::Rect(267, 33, 28, 34);
    areas[1] = cv::Rect(307, 33, 28, 34);
    areas[2] = cv::Rect(347, 33, 28, 34);

    const int factor[3] = { 100, 10, 1 };

    target = 0;

    for(int i=0; ret && i<3; i++)
    {
        cv::Mat4b ROI = image(areas[i]);

        std::vector<Segmentation::Component> components;

        Segmentation seg;
        seg.segment(ROI, components);

        int good_component = -1;
        int index = 0;
        for(Segmentation::Component& c : components)
        {
            if( c.gray_level > 0 && (good_component == -1 || components[good_component].pixels.size() < c.pixels.size()) )
            {
                good_component = index;
            }

            index++;
        }

        if(index >= 0)
        {
            const char character = extractCharacter(components[good_component].pixels);

            if( '0' <= character && character <= '9' )
            {
                target += factor[i] * int( character - '0' );
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }
    }

    return ret;
}

char LeCompteEstBonAgent::extractCharacter(const std::vector<cv::Point>& points)
{
    cv::Point min;
    min.x = points.front().x;
    min.y = points.front().y;

    for(const cv::Point& pt : points)
    {
        min.x = std::min<int>(min.x, pt.x);
        min.y = std::min<int>(min.y, pt.y);
    }

    cv::Mat1b normalized(40, 40);
    normalized = 0;

    for(const cv::Point& pt : points)
    {
        cv::Point opt = pt - min;

        if(opt.x < normalized.cols && opt.y < normalized.rows)
        {
            normalized(opt) = 255;
        }
    }

    // compute some features.

    const cv::Moments moments = cv::moments(normalized, true);
    const cv::Vec2f features(moments.m10, moments.m01);

    /*
    {
        static int i = 0;
        cv::imwrite("extract" + std::to_string(i) + ".png", normalized);
        std::ofstream f("feat_" + std::to_string(i) + ".txt");
        f << moments.m10 << std::endl;
        f << moments.m01 << std::endl;
        f.close();
        i++;
    }
    */

    bool first = true;
    char best_character = 0;
    float best_distance = 0.0;

    for(const Sample& s : mySamples)
    {
        const float distance = cv::norm(s.moments, features);

        if(first || distance < best_distance)
        {
            first = false;
            best_character = s.character;
            best_distance = distance;
        }
    }

    if(first)
    {
        throw std::logic_error("internal error");
    }

    return best_character;
}

