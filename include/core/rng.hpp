#ifndef GI_CORE_RANDOM_NUMBER_GENERATOR_HPP
#define GI_CORE_RANDOM_NUMBER_GENERATOR_HPP

#include "core/geometry.hpp"
#include <random>
#include <limits>

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

        static Vec3 Get3D()
        {
            return Vec3(Get1D(), Get1D(), Get1D());
        }

        static Vec3 Get3D(Float min, Float max)
        {
            return Vec3(Get1D(min, max), Get1D(min, max), Get1D(min, max));
        }

        static Vec3 GetInUnitDisk()
        {
            while (true)
            {
                auto p = Vec3(Get1D(-1, 1), Get1D(-1, 1), 0);
                if (p.LengthSquared() < 1)
                    return p;
            }
        }

        static Vec3 GetUnit3D()
        {
            while (true)
            {
                Vec3 v = Get3D(-1, 1);
                auto lensq = v.LengthSquared();
                if (1e-160 < lensq && lensq <= 1)
                    return v / sqrtf(lensq);
            }
        }

        static Vec3 GetOnHemisphere(Normal &n)
        {
            Vec3 onUnitShpere = GetUnit3D();
            if (Dot(onUnitShpere, n) > 0.0f)
                return onUnitShpere;
            else
                return -onUnitShpere;
        }

        static Vec3 GetCosDir()
        {
            auto r1 = Get1D();
            auto r2 = Get1D();

            auto phi = 2 * PI * r1;
            auto x = std::cos(phi) * std::sqrt(r2);
            auto y = std::sin(phi) * std::sqrt(r2);
            auto z = std::sqrt(1 - r2);

            return Vec3(x, y, z);
        }

        static Vec3 GetToSphere(Float r, Float distSq)
        {
            auto r1 = Get1D();
            auto r2 = Get1D();
            auto z = 1 + r2 * (std::sqrt(1 - r * r / distSq) - 1);

            auto phi = 2 * PI * r1;
            auto x = std::cos(phi) * std::sqrt(1 - z * z);
            auto y = std::sin(phi) * std::sqrt(1 - z * z);

            return Vec3(x, y, z);
        }
    };

    std::uniform_real_distribution<Float> RNG::distribution = std::uniform_real_distribution<Float>(0.0f, 1.0f);
    std::mt19937 RNG::generator;
}
#endif
