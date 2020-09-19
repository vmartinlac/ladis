#include <iostream>
#include <ctime>
#include "Utils.h"

std::string Utils::makeUTCISO8601DateTime(time_t t)
{
    struct tm tt;
    if(!gmtime_r(&t, &tt))
    //if(!localtime_r(&t, &tt))
    {
        std::cerr << "Could not retrieve UTC time!" << std::endl;
        exit(1);
    }

    char buffer[512];
    buffer[0] = 0;
    strftime(buffer, 512, "%FT%T", &tt);

    return buffer;
}

