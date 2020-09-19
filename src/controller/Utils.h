
#pragma once

#include <fstream>

namespace Utils
{
    template<typename RNG>
    void seedRNG(RNG& rng)
    {
        std::ifstream f("/dev/urandom", std::ifstream::binary);
        typename RNG::result_type s;
        f.read(reinterpret_cast<char*>(&s), sizeof(typename RNG::result_type));
        rng.seed(s);
    }

    std::string makeUTCISO8601DateTime(time_t t);
};

