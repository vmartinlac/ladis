#include <ostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "GraphVizHook.h"

GraphVizHook::GraphVizHook()
{
    myNextGraph = 0;
    myNextImage = 0;
}

std::string GraphVizHook::makeNodeName(int nodeid)
{
    return "n" + std::to_string(nodeid);
}

void GraphVizHook::hookCompletedNode(const Checkers::State& s, int nodeid, float value)
{
    std::stringstream image_path;
    image_path << "image_" << myNextImage << ".png";
    myNextImage++;

    cv::Mat3b image = makePicture(s);

    cv::imwrite(image_path.str(), image);

    const std::string node_name = makeNodeName(nodeid);

    myFile << node_name << " [";
    myFile << "label=<";
    myFile << "<table border=\"0px\">";

    myFile << "<tr>";
    myFile << "<td>";
    myFile << "<img src=\"" << image_path.str() << "\" />";
    myFile << "</td>";
    myFile << "</tr>";

    myFile << "<tr>";
    myFile << "<td>";
    if(s.isMyTurn())
    {
        myFile << "MAX";
    }
    else
    {
        myFile << "MIN";
    }
    myFile << "</td>";
    myFile << "</tr>";

    myFile << "<tr>";
    myFile << "<td>";
    myFile << value;
    myFile << "</td>";
    myFile << "</tr>";

    myFile << "</table>";
    myFile << ">";
    myFile << " shape=box style=filled";
    if(s.isMyTurn())
    {
        myFile << " fillcolor=green";
    }
    else
    {
        myFile << " fillcolor=red";
    }
    myFile << "]" << std::endl;
}

void GraphVizHook::hookFoundEdge(const Checkers::Action& action, int nodeid_from, int nodeid_to)
{
    const std::string node0 = makeNodeName(nodeid_from);
    const std::string node1 = makeNodeName(nodeid_to);

    myFile << node0 << " -> " << node1 << " [";
    myFile << "label=\"";
    for(int i=0; i<=action.getNumMoves(); i++)
    {
        if(i > 0)
        {
            myFile << '-';
        }
        myFile << action.getTrajectory(i);
    }
    myFile << "\"";
    myFile << "]" << std::endl;
}

void GraphVizHook::hookStarted()
{
    std::string filename = "graph_" + std::to_string(myNextGraph) + ".dot";
    myNextGraph++;

    myFile.open(filename.c_str());
    if(myFile.fail())
    {
        throw std::runtime_error("Could not open output graph file!");
    }

    myFile << "digraph {" << std::endl;
}

void GraphVizHook::hookFinished()
{
    myFile << "}" << std::endl;
    myFile.close();
}

cv::Mat1b GraphVizHook::makePicture(const Checkers::State& s)
{
    constexpr int SIDE = Checkers::SIDE;
    constexpr int N = Checkers::N;

    const int width = 128;
    const int height = 128;

    cv::Mat3b ret(height, width);

    ret = cv::Vec3b(16, 16, 16);

    for(int i=0; i<SIDE; i++)
    {
        for(int j=0; j<SIDE; j++)
        {
            const cv::Point A(
                j*width/SIDE,
                i*height/SIDE);

            const cv::Point B(
                (j+1)*width/SIDE,
                (i+1)*height/SIDE);

            const cv::Rect rect(A, B);

            const bool in_cell = (SIDE-1-j + (i % 2)) % 2 == 0;

            if(in_cell)
            {
                const int cell = (SIDE/2)*(SIDE-1-i) + (SIDE-1-j)/2;

                if(0 > cell || cell >= N)
                {
                    throw std::runtime_error("internal error");
                }

                ret(rect) = cv::Vec3b(64, 64, 64);

                if(s.readCell(cell) != '.')
                {
                    const cv::Point center(
                        (2*j+1)*width/(2*SIDE),
                        (2*i+1)*height/(2*SIDE));

                    cv::Vec3b color(255, 255, 255);
                    double thickness = 1;
                    double radius = 0.0;

                    switch(s.readCell(cell))
                    {
                    case 'o':
                    case 'O':
                        color = cv::Vec3b(227, 0, 255);
                        break;
                    case 'p':
                    case 'P':
                        color = cv::Vec3b(18, 220, 252);
                        break;
                    default:
                        throw std::runtime_error("internal error!");
                    }

                    radius = std::min(width/(2*SIDE), height/(2*SIDE)) - 2;

                    cv::circle(ret, center, radius, color, -1);

                    switch(s.readCell(cell))
                    {
                    case 'P':
                    case 'O':
                        cv::circle(ret, center, radius/2, cv::Vec3b(0,0,0), -1);
                        break;
                    }
                }
            }
        }
    }

    return ret;
}

