
#pragma once

#include <vector>
#include <set>
#include <opencv2/core.hpp>
#include <memory>

class Segmentation
{
public:

    struct Component
    {
        std::set<int> neighbors;
        uint8_t gray_level;
        std::vector<cv::Point> pixels;
    };

public:

    void segment(const cv::Mat4b& image, std::vector<Component>& components);
};

