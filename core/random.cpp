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

I32 Random::ValueI32(I32 min, I32 max)
{
    return I32(Value(F32(min), F32(max+1)));
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

Vec3 Random::CosineDir()
{
    auto r1 = Value();
    auto r2 = Value();

    auto phi = 2*pi*r1;
    auto x = std::cos(phi) * std::sqrt(r2);
    auto y = std::sin(phi) * std::sqrt(r2);
    auto z = std::sqrt(1-r2);

    return Vec3(x, y, z);
}
