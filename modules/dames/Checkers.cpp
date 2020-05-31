#include <sstream>
#include <iostream>
#include "Checkers.h"

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
    std::copy(grid, grid+N, myGrid);
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

float Checkers::State::getValue() const
{
    int my_count = 0;
    int his_count = 0;

    for(int i=0; i<N; i++)
    {
        switch(myGrid[i])
        {
        case 'p':
            my_count++;
            break;
        case 'P':
            my_count += 2;
            break;
        case 'o':
            his_count--;
            break;
        case 'O':
            his_count -= 2;
            break;
        case '.':
            break;
        default:
            throw std::runtime_error("internal error");
        }
    }

    const float end_game = 2*50+1;
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
        ret = static_cast<float>(my_count - his_count);
    }

    return ret;
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

    // Check whether there are ennemy pieces. If not, we have won and there is no action to take.

    const bool are_there_opponent_pieces = state.areThereOpponentPieces();

    // Enumerate all available moves disregarding whether they achieve maximum number of ennemy pieces removed.

    if(are_there_opponent_pieces)
    {
        struct Move
        {
            Move()
            {
                previous_move = -1;
                starting_cell = -1;
                landing_cell = -1;
                move_direction = -1;
                captured_cell = -1;
                processed = false;
            }

            bool processed;
            int previous_move;
            int starting_cell;
            int landing_cell;
            int move_direction;
            int captured_cell;
        };

        std::vector<Move> stack;
        int highest_capture_count = 0;

        for(int cell0=0; cell0<N; cell0++)
        {
            const char piece = state.readCell(cell0);

            if(piece == 'p' || piece == 'P')
            {
                stack.resize(1);
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

                        bool added = false;

                        // Enumerate available one-piece jumps.

                        if(piece == 'P')
                        {
                            // TODO
                            throw std::runtime_error("not implemented yet");
                        }
                        else if(piece == 'p')
                        {
                            for(int direction=0; direction<4; direction++)
                            {
                                const int cell2 = getReachable(cell1, direction);

                                if(cell2 >= 0 && (state.readCell(cell2) == 'o' || state.readCell(cell2) == 'O'))
                                {
                                    bool already_captured = false;

                                    {
                                        int k = this_index;

                                        while(!already_captured && k >= 0)
                                        {
                                            if(stack[k].captured_cell == cell2)
                                            {
                                                already_captured = true;
                                            }

                                            k = stack[k].previous_move;
                                        }
                                    }

                                    if(!already_captured)
                                    {
                                        const int cell3 = getReachable(cell2, direction);

                                        if(cell3 >= 0 && getReachable(cell3, direction) == '.')
                                        {
                                            stack.emplace_back();
                                            stack.back().previous_move = this_index;
                                            stack.back().captured_cell = cell2;
                                            stack.back().move_direction = direction;
                                            stack.back().landing_cell = cell3;
                                            stack.back().starting_cell = cell1;
                                            stack.back().processed = false;

                                            added = true;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            throw std::runtime_error("internal error");
                        }

                        if(!added)
                        {
                            int num_captured = 0;

                            {
                                int i = stack.size()-1;
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
                                    // TODO add to available moves even if (not to stack, directly to myAvailableActions).
                                }
                                else
                                {
                                    if(num_captured > highest_capture_count)
                                    {
                                        highest_capture_count = num_captured;
                                        myAvailableActions.clear();
                                    }

                                    myAvailableActions.emplace_back();
                                    AvailableAction& aa = myAvailableActions.back();

                                    {
                                        int trajectory[N];
                                        aa.action.set(num_captured, trajectory);
                                    }

                                    {
                                        char grid[N];

                                        for(int i=0; i<N; i++)
                                        {
                                            grid[i] = state.readCell(i);
                                        }

                                        {
                                            // TODO: update grid.
                                        }

                                        aa.state.setMyTurn(false);
                                        aa.state.setFlatGrid(grid);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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

