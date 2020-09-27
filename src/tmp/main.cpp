#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Controller.h>
#include <Checkers.h>
#include <Emulator.h>
#include <SaverAgent.h>
#include <ExperimentalAgent.h>

int main(int num_args, char** args)
{
    std::multimap<Checkers::Grid,Checkers::Grid> opponent_known_transitions;
    //db.loadKnownOpponentTransitions(3, opponent_known_transitions);

    Emulator em;
    ExperimentalAgent agent;
    Controller con;

    //con.run(&em, &agent, 0, 3);

    return 0;
}

