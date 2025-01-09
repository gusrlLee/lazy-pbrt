#pragma once

#include "math/perlin.h"

#include "core/image.h"
#include "core/interval.h"
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

class ImageTexture : public Texture
{
public:
    ImageTexture(Cstr *filename) : img(filename) {}

    Color Value(F32 u, F32 v, const Point3 &p) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (img.height() <= 0)
            return Color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).Clamp(u);
        v = 1.0 - Interval(0, 1).Clamp(v); // Flip V to image coordinates

        auto i = int(u * img.width());
        auto j = int(v * img.height());
        auto pixel = img.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    Image img;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(F32 scale) : scale(scale) {}
    Color Value(F32 u, F32 v, const Point3 &p) const override
    {
        return Color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.Turb(p, 7)));
    }

private:
    F32 scale;
    Perlin noise;
};