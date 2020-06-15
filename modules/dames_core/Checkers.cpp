#include <sstream>
#include <cmath>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Checkers.h"

cv::Mat1b Checkers::State::makePicture() const
{
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

                if(readCell(cell) != '.')
                {
                    const cv::Point center(
                        (2*j+1)*width/(2*SIDE),
                        (2*i+1)*height/(2*SIDE));

                    cv::Vec3b color(255, 255, 255);
                    double thickness = 1;
                    double radius = 0.0;

                    switch(readCell(cell))
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

                    switch(readCell(cell))
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

bool Checkers::State::areTherePlayerPieces() const
{
    const char* const begin = myGrid;
    const char* const end = myGrid+N;

    return std::find(begin, end, 'p') != end || std::find(begin, end, 'P') != end;
}

bool Checkers::State::areThereOpponentPieces() const
{
    const char* const begin = myGrid;
    const char* const end = myGrid+N;

    return std::find(begin, end, 'o') != end || std::find(begin, end, 'O') != end;
}

void Checkers::State::setMyTurn(bool my_turn)
{
    myIsMyTurn = my_turn;
}

void Checkers::State::setFlatGrid(const char* grid)
{
    for(int i=0; i<N; i++)
    {
        switch(grid[i])
        {
        case '.':
        case 'p':
        case 'P':
        case 'o':
        case 'O':
            myGrid[i] = grid[i];
            break;
        default:
            throw std::runtime_error("internal error");
        }
    }
}

void Checkers::State::setSquareGrid(const char* str)
{
    if( strlen(str) != (SIDE+1)*SIDE ) throw std::runtime_error("internal error");

    for(int i=0; i<N; i++)
    {
        const int zigzag = (2*i/SIDE)%2;
        const int j = 2*N-1 - (2*i + zigzag);
        const int k = j + j/SIDE;

        if(str[k] != ' ')
        {
            throw std::invalid_argument("Bad grid");
        }
    }

    for(int i=0; i<N; i++)
    {
        const int zigzag = 1 - (2*i/SIDE)%2;
        const int j = 2*N-1 - (2*i + zigzag);
        const int k = j + j/SIDE;

        switch(str[k])
        {
        case '.':
        case 'o':
        case 'O':
        case 'p':
        case 'P':
            break;
        default:
            throw std::invalid_argument("Bad grid");
        }

        myGrid[i] = str[k];
    }
}

char Checkers::State::readCell(int i) const
{
    return myGrid[i];
}

void Checkers::State::invert()
{
    myIsMyTurn = !myIsMyTurn;

    for(int i=0; i<N; i++)
    {
        switch(myGrid[i])
        {
        case 'o':
            myGrid[i] = 'p';
            break;
        case 'O':
            myGrid[i] = 'P';
            break;
        case 'p':
            myGrid[i] = 'o';
            break;
        case 'P':
            myGrid[i] = 'O';
            break;
        }
    }

    std::reverse(myGrid, myGrid+N);
}

std::string Checkers::State::getSquareGrid() const
{
    char buffer[(SIDE+1)*SIDE+1];

    buffer[SIDE*(SIDE+1)] = 0;

    for(int i=0; i<SIDE; i++)
    {
        buffer[(i+1)*(SIDE+1)-1] = '\n';
    }

    for(int i=0; i<N; i++)
    {
        const int zigzag = (2*i/SIDE)%2;
        const int j = 2*N-1 - (2*i + zigzag);
        const int k = j + j/SIDE;
        buffer[k] = ' ';
    }

    for(int i=0; i<N; i++)
    {
        const int zigzag = 1 - (2*i/SIDE)%2;
        const int j = 2*N-1 - (2*i + zigzag);
        const int k = j + j/SIDE;
        buffer[k] = myGrid[i];
    }

    return std::string(buffer);
}

bool Checkers::State::isMyTurn() const
{
    return myIsMyTurn;
}

std::string Checkers::Action::getText() const
{
    std::stringstream s;

    for(int i=0; i<=myNumMoves; i++)
    {
        if(i > 0)
        {
            s << " - ";
        }
        s << myTrajectory[i];
    }

    return s.str();
}

void Checkers::Action::set(int num_moves, int (&trajectory)[N])
{
    myNumMoves = num_moves;
    std::copy(trajectory, trajectory+N, myTrajectory);
}

int Checkers::Action::getNumMoves() const
{
    return myNumMoves;
}

int Checkers::Action::getTrajectory(int i) const
{
    return myTrajectory[i];
}

void Checkers::Action::invert()
{
    for(int i=0; i<=myNumMoves; i++)
    {
        myTrajectory[i] = (N-1-myTrajectory[i]);
    }
}

void Checkers::ActionIterator::init(const State& s)
{
    State state = s;

    if(s.isMyTurn() == false)
    {
        state.invert();
    }

    computeMoves(state);

    if(s.isMyTurn() == false)
    {
        for(AvailableAction& m : myAvailableActions)
        {
            m.action.invert();
            m.state.invert();
        }
    }
}

bool Checkers::ActionIterator::next(const State& state_from, Action& action, State& state_to)
{
    bool ret = false;

    if(myAvailableActions.empty() == false)
    {
        action = myAvailableActions.back().action;
        state_to = myAvailableActions.back().state;
        myAvailableActions.pop_back();
        ret = true;
    }

    return ret;
}

void Checkers::ActionIterator::computeMoves(const State& state)
{
    myAvailableActions.clear();

    const bool are_there_opponent_pieces = state.areThereOpponentPieces();

    if(are_there_opponent_pieces)
    {
        std::vector<Move> stack;
        int highest_capture_count = 0;

        for(int cell0=0; cell0<N; cell0++)
        {
            const char piece = state.readCell(cell0);

            if(piece == 'p' || piece == 'P')
            {
                stack.clear();
                stack.emplace_back();
                stack.back().landing_cell = cell0;

                while(stack.empty() == false)
                {
                    if(stack.back().processed)
                    {
                        stack.pop_back();
                    }
                    else
                    {
                        stack.back().processed = true;

                        const int cell1 = stack.back().landing_cell;
                        const int this_index = stack.size()-1;

                        const bool found_a_jump = enumerateJumpMoves(state, piece, stack, this_index);

                        if(!found_a_jump)
                        {
                            int num_captured = 0;

                            {
                                int i = this_index;
                                while(stack[i].previous_move >= 0)
                                {
                                    num_captured++;
                                    i = stack[i].previous_move;
                                }
                            }

                            if( num_captured >= highest_capture_count )
                            {
                                if( num_captured == 0 )
                                {
                                    enumerateNonJumpActions(cell1, state);
                                }
                                else
                                {
                                    if(num_captured > highest_capture_count)
                                    {
                                        myAvailableActions.clear();
                                        highest_capture_count = num_captured;
                                    }

                                    addActionFromStack(stack, this_index, state);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Checkers::ActionIterator::addActionFromStack(std::vector<Move>& stack, int index, const State& state)
{
    myAvailableActions.emplace_back();
    AvailableAction& aa = myAvailableActions.back();

    char grid[N];
    int trajectory[N];

    for(int i=0; i<N; i++)
    {
        grid[i] = state.readCell(i);
    }

    int count = 0;
    char piece = 0;
    int k = index;
    int last_k = index;
    while(k >= 0)
    {
        if(stack[k].captured_cell >= 0)
        {
            grid[stack[k].captured_cell] = '.';
        }
        trajectory[count] = stack[k].landing_cell;
        piece = stack[k].landing_cell;

        last_k = k;
        count++;
        k = stack[k].previous_move;
    }

    std::reverse(trajectory, trajectory+count);

    grid[stack[index].landing_cell] = grid[stack[last_k].landing_cell];
    grid[stack[last_k].landing_cell] = '.';

    for(int i=0; i<SIDE/2; i++)
    {
        if(grid[i] == 'o')
        {
            grid[i] = 'O';
        }

        if(grid[N-1-i] == 'p')
        {
            grid[N-1-i] = 'P';
        }
    }

    aa.action.set(count-1, trajectory);
    aa.state.setMyTurn(false);
    aa.state.setFlatGrid(grid);
}

bool Checkers::ActionIterator::enumerateNonJumpActions(int starting_cell, const State& state)
{
    const int piece = state.readCell(starting_cell);
    bool ret = false;

    if(piece != 'p' && piece != 'P')
    {
        throw std::runtime_error("internal error!");
    }

    const int directions[4] = { NEIGHBOR_TOP_RIGHT, NEIGHBOR_TOP_LEFT, NEIGHBOR_BOTTOM_RIGHT, NEIGHBOR_BOTTOM_LEFT };
    const int num_directions = (piece == 'P') ? 4 : 2;

    for(int idir=0; idir<num_directions; idir++)
    {
        const int direction = directions[idir];

        bool go_on = true;
        int landing_cell = starting_cell;

        while(go_on)
        {
            landing_cell = getReachable(landing_cell, direction);

            if(piece == 'p')
            {
                go_on = false;
            }

            if(landing_cell < 0)
            {
                go_on = false;
            }
            else if( state.readCell(landing_cell) == '.' )
            {
                myAvailableActions.emplace_back();
                AvailableAction& aa = myAvailableActions.back();

                int trajectory[N];
                char grid[N];

                for(int i=0; i<N; i++)
                {
                    grid[i] = state.readCell(i);
                }

                grid[starting_cell] = '.';
                grid[landing_cell] = piece;

                for(int i=0; i<SIDE/2; i++)
                {
                    if(grid[i] == 'o')
                    {
                        grid[i] = 'O';
                    }

                    if(grid[N-1-i] == 'p')
                    {
                        grid[N-1-i] = 'P';
                    }
                }

                trajectory[0] = starting_cell;
                trajectory[1] = landing_cell;

                aa.action.set(1, trajectory);
                aa.state.setMyTurn(false);
                aa.state.setFlatGrid(grid);

                ret = true;
            }
            else
            {
                go_on = false;
            }
        }
    }

    return ret;
}

bool Checkers::ActionIterator::enumerateJumpMoves(const State& state, char piece, std::vector<Move>& stack, int index)
{
    const int starting_cell = stack[index].landing_cell;
    bool ret = false;

    for(int direction=0; direction<4; direction++)
    {
        bool go_on = true;
        int current_cell = starting_cell;

        while(go_on)
        {
            const int adjacent_cell = getReachable(current_cell, direction);

            if(adjacent_cell < 0)
            {
                go_on = false;
            }
            else if( state.readCell(adjacent_cell) == 'o' || state.readCell(adjacent_cell) == 'O')
            {
                bool already_captured = false;

                go_on = false;

                {
                    int k = index;

                    while(!already_captured && k >= 0)
                    {
                        if(stack[k].captured_cell == adjacent_cell)
                        {
                            already_captured = true;
                        }

                        k = stack[k].previous_move;
                    }
                }

                if(!already_captured)
                {
                    bool go_on_bis = true;

                    int landing_cell = adjacent_cell;

                    while(go_on_bis)
                    {
                        landing_cell = getReachable(landing_cell, direction);

                        if(piece == 'p')
                        {
                            go_on_bis = false;
                        }

                        if(landing_cell < 0 || state.readCell(landing_cell) != '.')
                        {
                            go_on_bis = false;
                        }
                        else
                        {
                            stack.emplace_back();
                            stack.back().previous_move = index;
                            stack.back().captured_cell = adjacent_cell;
                            stack.back().move_direction = direction;
                            stack.back().landing_cell = landing_cell;
                            stack.back().starting_cell = starting_cell;
                            stack.back().processed = false;

                            ret = true;
                        }
                    }
                }
            }
            else if( piece == 'P')
            {
                current_cell = adjacent_cell;
            }
            else
            {
                go_on = false;
            }
        }
    }

    return ret;
}

int Checkers::getReachable(int from, int index)
{
    int ret = -1;

    static_assert(SIDE % 2 == 0);

    const int padding = 1 - (2*from/SIDE)%2;
    const int cell = 2*from + padding;
    const int a = cell % SIDE;
    const int b = cell / SIDE;

    const bool vertical[2] =
    {
        (b-1 >= 0),
        (b+1 < SIDE)
    };

    const bool horizontal[2] = 
    {
        (a-1 >= 0),
        (a+1 < SIDE)
    };

    const int delta[2] = { -1, 1 };

    const int i0 = (index >> 0) & 1;
    const int i1 = (index >> 1) & 1;

    if(horizontal[i0] && vertical[i1])
    {
        ret = (SIDE*(b+delta[i1]) + a + delta[i0]) / 2;
    }

    return ret;
}

float Checkers::PieceCountUtilityFunction::getValue(const State& state) const
{
    const float end_game = 10.0f*N + 1.0f;

    int my_count = 0;
    int his_count = 0;
    float delta = 0.0f;

    for(int i=0; i<N; i++)
    {
        switch(state.readCell(i))
        {
        case 'p':
            my_count++;
            delta += 1.0;
            break;
        case 'P':
            my_count++;
            delta += 2.1;
            break;
        case 'o':
            his_count++;
            delta -= 1.0;
            break;
        case 'O':
            his_count++;
            delta -= 2.1;
            break;
        case '.':
            break;
        default:
            throw std::runtime_error("internal error");
        }
    }

    float ret = 0.0f;

    if( my_count > 0 && his_count == 0 )
    {
        ret = end_game;
    }
    else if(his_count > 0 && my_count == 0)
    {
        ret = -end_game;
    }
    else
    {
        ret = delta;
    }

    return ret;
}

Checkers::NeuralNetworkUtilityFunction::NeuralNetworkUtilityFunction()
{
    myWeights.reset(new WeightTable());
    setDefaultWeights();
}

void Checkers::NeuralNetworkUtilityFunction::setDefaultWeights()
{
    static_assert(NUM_CELL_FEATURES >= 2);

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<6; j++)
        {
            for(int k=0; k<NUM_CELL_FEATURES; k++)
            {
                myWeights->local_weights_neighbors[i][j][k] = 0.0f;
            }
        }
    }

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<NUM_CELL_FEATURES; j++)
        {
            myWeights->local_weights_center[i][j] = 0.0f;
        }
    }

    for(int i=0; i<NUM_CELL_FEATURES; i++)
    {
        myWeights->local_biaises[i] = 0.0f;
    }

    for(int i=0; i<NUM_CELL_FEATURES; i++)
    {
        for(int j=0; j<N; j++)
        {
            myWeights->global_weights[i][j] = 0.0f;
        }
    }

    myWeights->local_weights_center[0][0] = 1.0f;
    myWeights->local_weights_center[1][0] = 3.0f;
    myWeights->local_weights_center[2][1] = 1.0f;
    myWeights->local_weights_center[3][1] = 3.0f;

    for(int i=0; i<N; i++)
    {
        myWeights->global_weights[0][i] = 1.0;
        myWeights->global_weights[1][i] = -1.0;
    }
}

int Checkers::NeuralNetworkUtilityFunction::getNumWeights() const
{
    int ret = 0;
    ret += 4*6*NUM_CELL_FEATURES;
    ret += 5*NUM_CELL_FEATURES;
    ret += NUM_CELL_FEATURES;
    ret += NUM_CELL_FEATURES*N;
    return ret;
}

void Checkers::NeuralNetworkUtilityFunction::setWeights(const std::vector<float>& weights)
{
    if(weights.size() != getNumWeights())
    {
        throw std::runtime_error("Incorrect weight size!");
    }

    int count = 0;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<6; j++)
        {
            for(int k=0; k<NUM_CELL_FEATURES; k++)
            {
                myWeights->local_weights_neighbors[i][j][k] = weights[count];
                count++;
            }
        }
    }

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<NUM_CELL_FEATURES; j++)
        {
            myWeights->local_weights_center[i][j] = weights[count];
            count++;
        }
    }

    for(int i=0; i<NUM_CELL_FEATURES; i++)
    {
        myWeights->local_biaises[i] = weights[count];
        count++;
    }

    for(int i=0; i<NUM_CELL_FEATURES; i++)
    {
        for(int j=0; j<N; j++)
        {
            myWeights->global_weights[i][j] = weights[count];
            count++;
        }
    }
}

void Checkers::NeuralNetworkUtilityFunction::getWeights(std::vector<float>& weights) const
{
    weights.resize(getNumWeights());

    int count = 0;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<6; j++)
        {
            for(int k=0; k<NUM_CELL_FEATURES; k++)
            {
                weights[count] = myWeights->local_weights_neighbors[i][j][k];
                count++;
            }
        }
    }

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<NUM_CELL_FEATURES; j++)
        {
            weights[count] = myWeights->local_weights_center[i][j];
            count++;
        }
    }

    for(int i=0; i<NUM_CELL_FEATURES; i++)
    {
        weights[count] = myWeights->local_biaises[i];
        count++;
    }

    for(int i=0; i<NUM_CELL_FEATURES; i++)
    {
        for(int j=0; j<N; j++)
        {
            weights[count] = myWeights->global_weights[i][j];
            count++;
        }
    }

    if(count != weights.size())
    {
        throw std::runtime_error("internal error");
    }
}

float Checkers::NeuralNetworkUtilityFunction::getValue(const State& state) const
{
    float ret = 0.0f;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<NUM_CELL_FEATURES; j++)
        {
            float local_score = myWeights->local_biaises[j];

            switch(state.readCell(i))
            {
            case 'p':
                local_score += myWeights->local_weights_center[0][j];
                break;
            case 'P':
                local_score += myWeights->local_weights_center[1][j];
                break;
            case 'o':
                local_score += myWeights->local_weights_center[2][j];
                break;
            case 'O':
                local_score += myWeights->local_weights_center[3][j];
                break;
            case '.':
                local_score += myWeights->local_weights_center[4][j];
                break;
            default:
                throw std::runtime_error("internal error");
            }

            for(int k=0; k<4; k++)
            {
                const int l = getReachable(i, k);

                if(l >= 0)
                {
                    switch(state.readCell(l))
                    {
                    case 'p':
                        local_score += myWeights->local_weights_neighbors[k][0][j];
                        break;
                    case 'P':
                        local_score += myWeights->local_weights_neighbors[k][1][j];
                        break;
                    case 'o':
                        local_score += myWeights->local_weights_neighbors[k][2][j];
                        break;
                    case 'O':
                        local_score += myWeights->local_weights_neighbors[k][3][j];
                        break;
                    case '.':
                        local_score += myWeights->local_weights_neighbors[k][4][j];
                        break;
                    default:
                        throw std::runtime_error("internal error");
                    }
                }
                else
                {
                    local_score += myWeights->local_weights_neighbors[k][5][j];
                }
            }

            local_score = std::max<float>(0.0, local_score);

            ret += myWeights->global_weights[j][i] * local_score;
        }
    }

    /*
    {
        float delta = 0.0f;

        for(int i=0; i<N; i++)
        {
            switch(state.readCell(i))
            {
            case 'p':
                delta += 1.0;
                break;
            case 'P':
                delta += 2.0;
                break;
            case 'o':
                delta -= 1.0;
                break;
            case 'O':
                delta -= 2.0;
                break;
            case '.':
                break;
            default:
                throw std::runtime_error("internal error");
            }
        }
    }
    */

    return ret;
}

void Checkers::Action::makeDebugEdgeSpec(std::ostream& s, const std::string& node0, const std::string& node1) const
{
    s << node0 << " -> " << node1 << " [";
    s << "label=\"";
    for(int i=0; i<=myNumMoves; i++)
    {
        if(i > 0)
        {
            s << '-';
        }
        s << myTrajectory[i];
    }
    s << "\"";
    s << "]" << std::endl;
}

void Checkers::State::makeDebugNodeSpec(std::ostream& s, const std::string& node_name, float value) const
{
    static int i=0;
    std::stringstream image_path;
    image_path << "image_" << i << ".png";
    i++;

    cv::Mat3b image = makePicture();

    cv::imwrite(image_path.str(), image);

    s << node_name << " [";
    s << "label=<";
    s << "<table border=\"0px\">";

    s << "<tr>";
    s << "<td>";
    s << "<img src=\"" << image_path.str() << "\" />";
    s << "</td>";
    s << "</tr>";

    s << "<tr>";
    s << "<td>";
    if(myIsMyTurn)
    {
        s << "MAX";
    }
    else
    {
        s << "MIN";
    }
    s << "</td>";
    s << "</tr>";

    s << "<tr>";
    s << "<td>";
    s << value;
    s << "</td>";
    s << "</tr>";

    s << "</table>";
    s << ">";
    s << " shape=box style=filled";
    if(myIsMyTurn)
    {
        s << " fillcolor=green";
    }
    else
    {
        s << " fillcolor=red";
    }
    s << "]" << std::endl;
}

