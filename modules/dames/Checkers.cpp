#include <iostream>
#include "Checkers.h"

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

    if(my_count > 0 && his_count == 0)
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


void Checkers::Action::setFrom(int from)
{
    myFrom = from;
}

void Checkers::Action::setTo(int to)
{
    myTo = to;
}

int Checkers::Action::getFrom() const
{
    return myFrom;
}

int Checkers::Action::getTo() const
{
    return myTo;
}

void Checkers::Action::invert()
{
    myFrom = (N-1-myFrom);
    myTo = (N-1-myTo);
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
        for(Move& m : myMoves)
        {
            m.action.invert();
            m.state.invert();
        }
    }
}

bool Checkers::ActionIterator::next(const State& state_from, Action& action, State& state_to)
{
    bool ret = false;

    if(myMoves.empty() == false)
    {
        action = myMoves.back().action;
        state_to = myMoves.back().state;
        myMoves.pop_back();
        ret = true;
    }

    return ret;
}

void Checkers::ActionIterator::computeMoves(const State& state)
{
    myMoves.clear();

    // first enumerate all moves.

    {
        int stack_size = 0;
        int stack[N];
        int next_direction[N];

        bool forbidden[N];

        for(int cell0=0; cell0<N; cell0++)
        {
            const char piece = state.readCell(cell0);

            if(piece == 'p' || piece == 'P')
            {
                std::fill(forbidden, forbidden+N, false);

                stack_size = 1;
                stack[0] = cell0;
                next_direction[0] = 0;

                while(stack_size > 0)
                {
                    const int cell1 = stack[stack_size-1];
                    int try_direction = -1;

                    if( 0 <= next_direction[stack_size-1] && next_direction[stack_size-1] < 4 )
                    {
                        forbidden[cell1] = true;
                        try_direction = next_direction[stack_size-1];
                        next_direction[stack_size-1]++;
                    }
                    else
                    {
                        stack_size--;
                        forbidden[cell1] = false;
                        try_direction = -1;
                    }

                    if(try_direction >= 0)
                    {
                        const int cell2 = getReachable(cell1, try_direction);

                        if(cell2 >= 0)
                        {
                            if( state.readCell(cell2) == ' ' && (piece == 'P' || (try_direction & 2)) )
                            {
                            }

                            // TODO
                        }
                    }
                }
            }
        }
    }

    // second remove moves which do not result in maximum number of opponent pieces removed.

    {
        int max_removed = 0;
        for(const Move& move : myMoves)
        {
            max_removed = std::max(max_removed, move.num_eliminated_pieces);
        }

        std::vector<Move>::iterator it = myMoves.begin();
        while(it != myMoves.end())
        {
            if(it->num_eliminated_pieces == max_removed)
            {
                it++;
            }
            else
            {
                it = myMoves.erase(it);
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

