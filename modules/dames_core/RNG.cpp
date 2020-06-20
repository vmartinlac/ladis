#include <fstream>
#include "RNG.h"

RNG::RNG()
{
    seedFromOS();
}

void RNG::seedFromOS()
{
    std::ifstream f("/dev/urandom");
    unsigned int seed = 0;
    f.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    if(!f.good())
    {
        throw std::runtime_error("error while getting RNG seed from /dev/urandom");
    }
    f.close();

    myEngine.seed(seed);
}

void RNG::seed(int seed)
{
    myEngine.seed(seed);
}

void RNG::autoseed()
{
    seedFromOS();
}

