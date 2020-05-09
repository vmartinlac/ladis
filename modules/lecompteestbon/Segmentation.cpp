#include <stack>
#include <opencv2/imgproc.hpp>
#include "Segmentation.h"
#include "UnionFind.h"

void Segmentation::segment(const cv::Mat4b& image, std::vector<Component>& components)
{
    cv::Mat1b gray;
    cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);

    UnionFind uf;
    uf.init(image.cols*image.rows);

    for(int i=0; i<image.rows-1; i++)
    {
        for(int j=0; j<image.cols; j++)
        {
            if(gray(i,j) == gray(i+1,j))
            {
                uf.join( i*image.cols+j, (i+1)*image.cols+j );
            }
        }
    }

    for(int i=0; i<image.rows; i++)
    {
        for(int j=0; j<image.cols-1; j++)
        {
            if(gray(i,j) == gray(i,j+1))
            {
                uf.join( i*image.cols+j, i*image.cols+j+1 );
            }
        }
    }

    std::vector<int> labels;
    const int N = uf.getComponents(labels);

    components.clear();
    components.resize(N);

    /*
    static int aa = 0;
    aa++;
    cv::imwrite("aa" + std::to_string(aa) + ".png", gray);
    */

    for(int i=0; i<image.rows; i++)
    {
        for(int j=0; j<image.cols; j++)
        {
            Component& c = components[labels[i*image.cols+j]];
            c.gray_level = gray(i,j);
            c.pixels.emplace_back(j,i);
        }
    }

    for(int i=0; i<image.rows-1; i++)
    {
        for(int j=0; j<image.cols; j++)
        {
            const int l1 = labels[i*image.cols+j];
            const int l2 = labels[(i+1)*image.cols+j];

            if(l1 != l2)
            {
                components[l1].neighbors.insert(l2);
                components[l2].neighbors.insert(l1);
            }
        }
    }

    for(int i=0; i<image.rows; i++)
    {
        for(int j=0; j<image.cols-1; j++)
        {
            const int l1 = labels[i*image.cols+j];
            const int l2 = labels[i*image.cols+j+1];

            if(l1 != l2)
            {
                components[l1].neighbors.insert(l2);
                components[l2].neighbors.insert(l1);
            }
        }
    }
}

