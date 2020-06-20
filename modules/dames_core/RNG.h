
#pragma once

#include <mutex>
#include <random>
#include <memory>

class RNG
{
public:

    using UnderlyingEngine = std::default_random_engine;
    using result_type = UnderlyingEngine::result_type;

    RNG();

    void seed(int seed);

    void autoseed();

    result_type max()
    {
        return myEngine.max();
    }

    result_type min()
    {
        return myEngine.min();
    }

    result_type operator()()
    {
        std::unique_lock<std::mutex> lock(myMutex);
        return myEngine();
    }

protected:

    void seedFromOS();

protected:

    UnderlyingEngine myEngine;
    std::mutex myMutex;
};

using RNGPtr = std::shared_ptr<RNG>;

