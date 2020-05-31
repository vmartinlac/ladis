
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

        bool areTherePlayerPieces() const;
        bool areThereOpponentPieces() const;
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

        int myNumMoves;
        int myTrajectory[N];

    public:

        void set(int num_moves, int (&trajectory)[N]);
        int getNumMoves() const;
        int getTrajectory(int i) const;
        void invert();
        std::string getText() const;
    };

    class ActionIterator
    {
    public:

        void init(const State& s);
        bool next(const State& state_from, Action& action, State& state_to);

    protected:

        void computeMoves(const State& s);

    protected:

        struct AvailableAction
        {
            Action action;
            State state;
        };

    protected:

        std::vector<AvailableAction> myAvailableActions;
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

