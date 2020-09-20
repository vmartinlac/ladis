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
#include <MySQLDatabase.h>
#include <Emulator.h>
#include <SaverAgent.h>
#include <UniformAgent.h>

void play_match(int match_id)
{
    Emulator em;
    UniformAgent agent;
    SaverAgent saver_agent(&agent);
    Controller con;

    con.run(&em, &saver_agent, true, 0);

    MatchLogPtr match_log = saver_agent.retrieveMatchLog();

    if(match_log)
    {
        {
            std::stringstream filename;
            filename << "log_" << match_id << ".json";
            match_log->saveJson(filename.str());
        }

        if(mysql_library_init(0, nullptr, nullptr))
        {
            std::cout << "Could not initialize mysqlclient library!" << std::endl;
            exit(1);
        }

        {
            MySQLDatabase db;
            db.saveMatch(*match_log);
        }

        mysql_library_end();
    }
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

    using RNG = std::default_random_engine;
    RNG rng;

    {
        std::ifstream f("/dev/urandom", std::ifstream::binary);
        RNG::result_type s;
        f.read(reinterpret_cast<char*>(&s), sizeof(RNG::result_type));
        rng.seed(s);
    }

    std::map<int,int> pid_to_match_index;

    auto wait_one_child = [&pid_to_match_index] ()
    {
        const int child_pid = wait(nullptr);
        std::map<int,int>::iterator it = pid_to_match_index.find(child_pid);

        if(it == pid_to_match_index.end())
        {
            std::cerr << "Internal error" << std::endl;
            exit(1);
        }

        std::cout << "Matched " << it->second << " ended." << std::endl;

        pid_to_match_index.erase(it);
    };

    bool exit_as_child = false;

    for(int match_index=0; !exit_as_child && match_index<num_matches; match_index++)
    {
        if(pid_to_match_index.size() >= num_processes)
        {
            wait_one_child();
        }

        const int child_pid = fork();

        if(child_pid == 0)
        {
            play_match(match_index);
            exit_as_child = true;
        }
        else
        {
            if(pid_to_match_index.find(child_pid) != pid_to_match_index.end())
            {
                std::cerr << "Internal error" << std::endl;
                exit(1);
            }

            pid_to_match_index[child_pid] = match_index;
            std::cout << "Match " << match_index << " started." << std::endl;
        }
    }

    if(!exit_as_child)
    {
        while(!pid_to_match_index.empty())
        {
            wait_one_child();
        }
    }

    return 0;
}

