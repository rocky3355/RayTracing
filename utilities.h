#pragma once
#include <array>
#include <cmath>
#include <limits>
#include <random>

namespace raytracing
{
const double Infinity = std::numeric_limits<double>::infinity();

// TODO: Move to a class
constexpr std::size_t RANDOM_NUMBER_OF_VALUES{ 1000000U };
static std::array<double, RANDOM_NUMBER_OF_VALUES> random_double_cache;
static std::uint16_t random_counter = 0;
static bool is_random_initialized{ false };

inline void InitRandom()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    
    double value = 0.0;
    for (std::size_t i = 0; i < RANDOM_NUMBER_OF_VALUES; ++i)
    {
        random_double_cache.at(i) = distribution(generator);
    }
    is_random_initialized = true;

    std::cout << "Random initialized. Number of values: " << RANDOM_NUMBER_OF_VALUES << std::endl;
}

// TODO: all inline?
inline double GetRandomDouble()
{
    if (!is_random_initialized)
    {
        InitRandom();
    }
    double value = random_double_cache.at(random_counter++);
    if (random_counter == RANDOM_NUMBER_OF_VALUES)
    {
        random_counter = 0;
    }
    return value;
}

inline double GetRandomDouble(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * GetRandomDouble();
}

// TODO: Check if this is working
inline int GetRandomInt(int min, int max)
{
    return static_cast<int>(GetRandomDouble(min, max + 1));
}

inline double Clamp(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double DegreesToRadians(double value)
{
    return value / 180.0 * M_PI;
}
}  // namespace raytracing
