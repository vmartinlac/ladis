#include <iostream>
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

    if(mysql_library_init(0, nullptr, nullptr))
    {
        std::cout << "Could not initialize mysqlclient library!" << std::endl;
        exit(1);
    }

    MySQLDatabase db;
    Emulator em;
    UniformAgent agent;
    SaverAgent saver_agent(&db, &agent);
    Controller con;

    con.run(&em, &saver_agent, false, 0);

    mysql_library_end();
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

