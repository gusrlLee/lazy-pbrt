#pragma once
#include "core/types.h"
#include "math/vec3.h"
#include "core/color.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual Color Value(F32 u, F32 v, const Point3 &p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(const Color &albedo) : albedo(albedo) {}
    SolidColor(F32 red, F32 green, F32 blue) : SolidColor(Color(red, green, blue)) {}
    Color Value(F32 u, F32 v, const Point3 &p) const override { return albedo; }

private:
    Color albedo;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture(F32 scale, Sptr<Texture> even, Sptr<Texture> odd) : invScale(1.0 / scale), even(even), odd(odd) {}
    CheckerTexture(F32 scale, const Color &c1, const Color &c2) : CheckerTexture(scale, MakeSptr<SolidColor>(c1), MakeSptr<SolidColor>(c2)) {}

    Color Value(F32 u, F32 v, const Point3 &p) const override 
    {
        auto xInt = I32(std::floor(invScale * p.x()));
        auto yInt = I32(std::floor(invScale * p.y()));
        auto zInt = I32(std::floor(invScale * p.z()));

        bool isEven = (xInt + yInt + zInt) % 2 == 0;
        return isEven ? even->Value(u, v, p) : odd->Value(u, v, p);
    }


private:
    F32 invScale;
    Sptr<Texture> even;
    Sptr<Texture> odd;
};