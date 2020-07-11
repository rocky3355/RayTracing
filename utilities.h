#pragma once
#include <cmath>
#include <limits>
#include <random>

namespace raytracing
{
const double Infinity = std::numeric_limits<double>::infinity();

// TODO: all inline?
inline double GetRandomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
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
