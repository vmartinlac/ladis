#include <LADIS.h>
#include "Agent.h"

int main(int num_args, char** args)
{
    LeCompteEstBonAgent agent;

    LADIS::run(&agent, true);

    return 0;
}

