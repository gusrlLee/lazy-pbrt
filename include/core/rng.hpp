#ifndef GI_CORE_RANDOM_NUMBER_GENERATOR_HPP
#define GI_CORE_RANDOM_NUMBER_GENERATOR_HPP

#include "core/geometry.hpp"
#include <random>

namespace gi
{
    // Random Number Generator
    class RNG
    {
    public:
        static std::uniform_real_distribution<Float> distribution;
        static std::mt19937 generator;

        static Float Get1D()
        {
            return distribution(generator);
        }

        static Float Get1D(Float min, Float max)
        {
            return min + (max - min) * Get1D();
        }

        static Int Get1D(Int min, Int max)
        {
            return Int(Get1D(Float(min), Float(max + 1)));
        }

        static Vec2f Get2D()
        {
            Vec2f v = {Get1D(), Get1D()};
            return v;
        }

        static Vec2f Get2D(Float min, Float max)
        {
            Vec2f v = {Get1D(min, max), Get1D(min, max)};
            return v;
        }

        static Vec3f Get3D()
        {
            Vec3 v = {Get1D(), Get1D(), Get1D()};
            return v;
        }

        static Vec3f Get3D(Float min, Float max)
        {
            Vec3 v = {Get1D(min, max), Get1D(min, max), Get1D(min, max)};
            return v;
        }
    };

    std::uniform_real_distribution<Float> RNG::distribution = std::uniform_real_distribution<Float>(0.0f, 1.0f);
    std::mt19937 RNG::generator;
}
#endif