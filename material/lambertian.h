#pragma once
#include "core/types.h"
#include "math/vec3.h"

#include "core/material.h"
#include "core/color.h"

#include "core/random.h"
#include "core/texture.h"

class Lambertian : public Material
{
public:
    Lambertian(const Color &albedo) : tex(MakeSptr<SolidColor>(albedo)) {}
    Lambertian(Sptr<Texture> tex) : tex(tex) {}
    
    bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut, F32 &pdf) const override;

    F32 ScatteringPdf(const Ray &rIn, const HitRecord &rec, const Ray &rOut) const override;

private:
    // Color albedo;
    Sptr<Texture> tex;
};