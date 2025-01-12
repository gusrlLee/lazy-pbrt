#pragma once

#include "core/color.h"
#include "core/material.h"
#include "core/texture.h"

class Isotropic : public Material
{
public:
    Isotropic(const Color &albedo) :
        tex(MakeSptr<SolidColor>(albedo)) {}
    Isotropic(Sptr<Texture> tex) : tex(tex) {}
    
    bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut, F32 &pdf) const override
    {
        rOut = Ray(rec.P, Random::UnitVector3(), rIn.Time());
        attenuation = tex->Value(rec.u, rec.v, rec.P);
        pdf = 1 / (4 * pi);
        return true;
    }

    F32 ScatteringPdf(const Ray& rIn, const HitRecord &rec, const Ray &rOut) const override
    {
        return 1 / (4 * pi);
    }

private:
    Sptr<Texture> tex;
};