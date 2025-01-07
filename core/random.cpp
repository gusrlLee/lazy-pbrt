#include "core/random.h"
#include "random.h"

std::uniform_real_distribution<F32> Random::distribution = std::uniform_real_distribution<F32>(0.0, 1.0f);
std::mt19937 Random::generator; 

F32 Random::Value()
{
    return distribution(generator);
}

F32 Random::Value(F32 min, F32 max)
{
    return min + (max - min) * Value();
}

Vec3 Random::Vector3()
{
    return Vec3(Value(), Value(), Value());
}

Vec3 Random::Vector3(F32 min, F32 max)
{
    return Vec3(Value(min, max), Value(min, max), Value(min, max));
}

Vec3 Random::UnitVector3()
{
    while (true)
    {
        auto p = Vector3(-1, 1);
        auto lensq = p.LengthSqaured();
        if (1e-160 < lensq && lensq <= 1.0)
            return p / sqrt(lensq);
    }
}

Vec3 Random::InUnitDisk()
{
    while (true)
    {
        auto p = Vec3(Value(-1, 1), Value(-1, 1), 0);
        if (p.LengthSqaured() < 1)
            return p;
    }
}

Vec3 Random::OnHemisphere(const Vec3 &normal)
{
    auto on_unit_sphere = UnitVector3();
    if (Dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Color Random::GenColor()
{
    return Color(Value(), Value(), Value());
}

Color Random::GenColor(F32 min, F32 max)
{
    return Color(Value(min, max), Value(min, max), Value(min, max));
}
