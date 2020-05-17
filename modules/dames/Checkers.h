
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

    static void initialize();

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

    protected:

        void setGrid(const char* str);

    public:

        void init(const char* grid, bool my_turn);
        char readCell(int i) const;
        void invert();
        std::string getGrid() const;
        bool isMyTurn() const;
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

        void computeMovesFromMan(const State& s, int i);

        void computeMovesFromQueen(const State& s, int i);

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

    static bool staHasLeftTop[N];
    static int staLeftTop[N];

    static bool staHasRightTop[N];
    static int staRightTop[N];

    static bool staHasLeftBottom[N];
    static int staLeftBottom[N];

    static bool staHasRightBottom[N];
    static int staRightBottom[N];
};

