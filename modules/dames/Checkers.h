
#pragma once

#include <vector>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include "Minimax.h"

class Checkers
{
public:

    static constexpr int SIDE = 10;
    static constexpr int N = (SIDE*SIDE)/2;

    class State
    {
    protected:

        /*
        p means players'man
        P means players'queen
        o means opponent's man
        O means opponent's queen
        . means empty
        */
        char myGrid[N];
        bool myIsMyTurn;

    public:

        void setMyTurn(bool my_turn);
        void setFlatGrid(const char* grid);
        void setSquareGrid(const char* grid);

        char readCell(int i) const;
        std::string getSquareGrid() const;
        bool isMyTurn() const;

        void invert();

        float getValue() const;
    };

    class Action
    {
    protected:

        int myFrom;
        int myTo;

    public:

        void setFrom(int from);
        void setTo(int to);
        int getFrom() const;
        int getTo() const;
        void invert();
    };

    class ActionIterator
    {
    public:

        void init(const State& s);

        bool next(const State& state_from, Action& action, State& state_to);

    protected:

        void computeMoves(const State& s);

    protected:

        struct Move
        {
            Move()
            {
                num_eliminated_pieces = 0;
            }

            int num_eliminated_pieces;
            Action action;
            State state;
        };

    protected:

        std::vector<Move> myMoves;
    };

    using Solver = Minimax<State, Action, ActionIterator>;

protected:

    enum Neighbor
    {
        NEIGHBOR_BOTTOM_RIGHT=0,
        NEIGHBOR_BOTTOM_LEFT=1,
        NEIGHBOR_TOP_RIGHT=2,
        NEIGHBOR_TOP_LEFT=3,
    };

protected:

    static int getReachable(int from, int index);
};

