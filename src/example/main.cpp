#include <iostream>
#include <random>
#include <thread>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Emulator.h"
#include "Controller.h"
#include "Checkers.h"
#include "Database.h"

class CheckersAgent : public Agent
{
public:

    std::string getName() override
    {
        return "minimax";
    }

    void beginMatch() override
    {
    }

    virtual bool play(const Checkers::State& state, Checkers::Action& action) = 0;

    bool play(const State& state_, Action& action_) override
    {
        Checkers::State state;
        Checkers::Action action;

        state.setMyTurn(true);
        state.setFlatGrid(state_.checkerboard);

        const bool ok = play(state, action);

        if(ok)
        {
            action_.trajectory.resize(action.getNumMoves()+1);

            for(int i=0; i<=action.getNumMoves(); i++)
            {
                action_.trajectory[i] = action.getTrajectory(i);
            }
        }

        return ok;
    }

    void endMatch(int result) override
    {
        std::ofstream log("result.txt");
        log << result << std::endl;
    }
};

class UniformAgent : public CheckersAgent
{
public:

    bool play(const Checkers::State& state, Checkers::Action& action) override
    {
        bool ret = false;

        std::vector<Checkers::Action> actions;

        Checkers::enumerateActions(state, actions);

        if(!actions.empty())
        {
            action = actions[myRNG() % actions.size()];
            ret = true;
        }

        return ret;
    }

protected:

    std::default_random_engine myRNG;
};

class MinimaxAgent : public CheckersAgent
{
public:

    bool play(const Checkers::State& state, Checkers::Action& action) override
    {
        Checkers::Solver solver;
        Checkers::State new_state;
        Checkers::UtilityFunction utility;

        return solver.solve(state, action, new_state, utility, 4);
    }
};

void play_match(int match_id)
{
    std::stringstream directory_ss;
    directory_ss << "match_" << std::setw(5) << std::setfill('0') << match_id;

    const std::string directory = directory_ss.str();

    if( mkdir(directory.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) < 0 )
    {
        // TODO
    }

    if( chdir(directory.c_str()) < 0 )
    {
        std::cerr << "Could not change current directory!" << std::endl;
        exit(1);
    }

    Database db("database.sqlite");
    Emulator em;
    UniformAgent agent;
    Controller con;

    con.run(&em, &agent, false, 0);
}

int main(int num_args, char** args)
{
    if(num_args != 3)
    {
        std::cerr << "Bad number of arguments!" << std::endl;
        exit(1);
    }

    const int num_matches = atoi(args[1]);
    const int num_processes = atoi(args[2]);

    if(num_matches < 0 || num_processes < 0)
    {
        std::cerr << "Bad input values" << std::endl;
        exit(1);
    }

    int running = 0;
    bool exit_as_child = false;

    for(int match_index=0; !exit_as_child && match_index<num_matches; match_index++)
    {
        if(running >= num_processes)
        {
            wait(nullptr);
            running--;
        }

        if(fork() == 0)
        {
            play_match(match_index);
            exit_as_child = true;
        }
        else
        {
            running++;
        }
    }

    if(!exit_as_child)
    {
        while(running > 0)
        {
            wait(nullptr);
            running--;
        }
    }

    return 0;
}

