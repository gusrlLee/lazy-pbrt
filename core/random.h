#pragma once 
#include <random>
#include <limits>

#include "core/types.h"
#include "math/vec3.h"
#include "core/color.h"

class Random 
{
public:
    static F32 Value();
    static F32 Value(F32 min, F32 max);
    static Vec3 Vector3();
    static Vec3 Vector3(F32 min, F32 max);
    static Vec3 UnitVector3();
    static Vec3 InUnitDisk();
    static Vec3 OnHemisphere(const Vec3 &normal);

    static Color GenColor();
    static Color GenColor(F32 min, F32 max);

private:
    static std::uniform_real_distribution<F32> distribution;
    static std::mt19937 generator;
};