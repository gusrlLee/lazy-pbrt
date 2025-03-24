#ifndef NANO_GLOBAL_ILLUMINATION_SAMPLE_HPP
#define NANO_GLOBAL_ILLUMINATION_SAMPLE_HPP

#include "nanogi/data.hpp"
#include <random>

namespace nanogi
{
    class Random
    {
    public:
        static std::uniform_real_distribution<Float> distribution;
        static std::mt19937 generator;

        // return [0, 1]
        inline static Float Get1D()
        {
            return distribution(generator);
        }

        inline static Float Get1D(Float min, Float max)
        {
            return min + (max - min) * Get1D();
        }

        inline static Vec2 Get2D()
        {
            Vec2 v = {Get1D(), Get1D()};
            return v;
        }

        inline static Vec2 Get2D(Float min, Float max)
        {
            Vec2 v = {Get1D(min, max), Get1D(min, max)};
            return v;
        }

        inline static Vec3 Get3D()
        {
            Vec3 v = {Get1D(), Get1D(), Get1D()};
            return v;
        }

        inline static Vec3 Get3D(Float min, Float max)
        {
            Vec3 v = {Get1D(min, max), Get1D(min, max), Get1D(min, max)};
            return v;
        }

        static Float UnitFloat()
        {
            return Get1D(0, 1);
        }

        static Vec2 UnitVec2()
        {
            while (true)
            {
                auto p = Get2D(-1, 1);
                auto lenSq = p.LenSq();
                if (Eps < lenSq && lenSq <= 1)
                    return p / std::sqrt(lenSq);
            }
        }

        static Vec3 UnitVec3()
        {
            while (true)
            {
                auto p = Get3D(-1, 1);
                auto lenSq = p.LenSq();
                if (Eps < lenSq && lenSq <= 1)
                    return p / std::sqrt(lenSq);
            }
        }

        static Vec3 UnitDisk()
        {
            while (true)
            {
                auto p = Vec3(Get1D(-1, 1), Get1D(-1, 1), 0);
                if (p.LenSq() < 1)
                    return p;
            }
        }

        static Vec3 OnHemisphere(const Vec3 &normal)
        {
            Vec3 onUnitSphere = UnitVec3();
            if (Dot(onUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
                return onUnitSphere;
            else
                return -onUnitSphere;
        }

        static Vec3 CosineDirection()
        {
            auto r1 = Get1D();
            auto r2 = Get1D();

            auto phi = 2 * Pi * r1;
            auto x = std::cos(phi) * std::sqrt(r2);
            auto y = std::sin(phi) * std::sqrt(r2);
            auto z = std::sqrt(1 - r2);

            return Vec3(x, y, z);
        }
    };

    std::uniform_real_distribution<Float> Random::distribution = std::uniform_real_distribution<Float>(0.0f, 1.0f);
    std::mt19937 Random::generator;
}

#endif