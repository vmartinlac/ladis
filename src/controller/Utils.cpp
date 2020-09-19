#include <iostream>
#include <ctime>
#include "Utils.h"

std::string Utils::getCurrentDateTime()
{
    char buffer[512];
    time_t t = time(nullptr);
    struct tm tt;
    if(!gmtime_r(&t, &tt))
    {
        std::cerr << "Could not retrieve UTC time!" << std::endl;
        abort();
    }

    buffer[0] = 0;
    strftime(buffer, 512, "%FT%T", &tt);

    return buffer;
}

