#ifndef GI_CORE_TEXTURE_HPP
#define GI_CORE_TEXTURE_HPP

#include "core/image.hpp"
#include "core/perlin.h"

namespace gi
{

    class Texture
    {
    public:
        virtual ~Texture() = default;
        virtual Color Value(Float u, Float v, const Point3 &p) const = 0;
    };

    class SolidColor : public Texture
    {
    public:
        SolidColor(const Color &albedo) : albedo(albedo) {}
        SolidColor(Float red, Float green, Float blue) : SolidColor(Color(red, green, blue)) {}
        Color Value(Float u, Float v, const Point3 &p) const override { return albedo; }

    private:
        Color albedo;
    };

    class CheckerTexture : public Texture
    {
    public:
        CheckerTexture(Float scale, SPtr<Texture> even, SPtr<Texture> odd) : invScale(1.0 / scale), even(even), odd(odd) {}
        CheckerTexture(Float scale, const Color &c1, const Color &c2) : CheckerTexture(scale, MakeShared<SolidColor>(c1), MakeShared<SolidColor>(c2)) {}

        Color Value(Float u, Float v, const Point3 &p) const override
        {
            auto xInt = Int(std::floor(invScale * p.x()));
            auto yInt = Int(std::floor(invScale * p.y()));
            auto zInt = Int(std::floor(invScale * p.z()));

            bool isEven = (xInt + yInt + zInt) % 2 == 0;
            return isEven ? even->Value(u, v, p) : odd->Value(u, v, p);
        }

    private:
        Float invScale;
        SPtr<Texture> even;
        SPtr<Texture> odd;
    };

    class ImageTexture : public Texture
    {
    public:
        ImageTexture(CString *filename) : img(filename) {}

        Color Value(Float u, Float v, const Point3 &p) const override
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
        NoiseTexture(Float scale) : scale(scale) {}
        Color Value(Float u, Float v, const Point3 &p) const override
        {
            return Color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.Turb(p, 7)));
        }

    private:
        Float scale;
        Perlin noise;
    };
}

#endif