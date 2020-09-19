
#pragma once

#include <random>
#include <memory>
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

    static int flatIndexToGridIndex(int i);
    static int gridIndexToFlatIndex(int i);

    using Grid = std::array<char,N>;

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
        void setGrid(const Grid& grid);
        char readCell(int i) const;
        std::string getSquareGrid() const;
        std::string getFlatGrid() const;
        bool isMyTurn() const;
        void invert();
        bool operator==(const State& other) const;
        bool operator!=(const State& other) const;
        bool operator<(const State& other) const;
    };

    class UtilityFunction
    {
    public:

        UtilityFunction();
        void setFromCrossover(const UtilityFunction& parent0, const UtilityFunction& parent1, std::default_random_engine& engine);
        void setDefaultWeights();
        int getNumWeights() const;
        void setWeights(const std::vector<float>& weights);
        void getWeights(std::vector<float>& weights) const;
        float getValue(const State& state) const;

    protected:

        static constexpr int NUM_CELL_FEATURES = 2;

        struct WeightTable
        {
            // order: "jJoO.E"
            float local_weights_center[5][NUM_CELL_FEATURES];
            float local_weights_neighbors[4][6][NUM_CELL_FEATURES];
            float local_biaises[NUM_CELL_FEATURES];
            float global_weights[NUM_CELL_FEATURES][N];
        };

    protected:

        std::unique_ptr<WeightTable> myWeights;
    };

    class Action
    {
    protected:

        int myNumMoves;
        int myTrajectory[N];

    public:

        int getNumCells() const;
        int getCell(int i) const;
        void set(int num_moves, int (&trajectory)[N]);
        int getNumMoves() const;
        int getTrajectory(int i) const;
        void invert();
        std::string getText() const;
        void makeDebugEdgeSpec(std::ostream& s, const std::string& node0, const std::string& node1) const;
        bool operator==(const Action& other) const;
        bool operator!=(const Action& other) const;
        bool operator<(const Action& other) const;
    };

    class ActionIterator
    {
    public:

        void init(const State& s);
        bool next(const State& state_from, Action& action, State& state_to);

    protected:

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

        struct AvailableAction
        {
            Action action;
            State state;
        };

    protected:

        void computeMoves(const State& s);
        bool enumerateJumpMoves(const State& state, char piece, std::vector<Move>& stack, int index);
        bool enumerateNonJumpActions(int starting_cell, const State& state);
        void addActionFromStack(std::vector<Move>& stack, int index, const State& state);

    protected:

        std::vector<AvailableAction> myAvailableActions;
    };

    static void enumerateActions(
        const State& state,
        std::vector< std::tuple<Action,State> >& action_states);

    static void enumerateActions(
        const State& state,
        std::vector<Action>& actions);

    using Solver = Minimax<State, UtilityFunction, Action, ActionIterator>;

public:

    enum Neighbor
    {
        NEIGHBOR_BOTTOM_RIGHT=0,
        NEIGHBOR_BOTTOM_LEFT=1,
        NEIGHBOR_TOP_RIGHT=2,
        NEIGHBOR_TOP_LEFT=3,
    };

public:

    static int getReachable(int from, int index);
};

