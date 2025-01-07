#pragma once 

#include "core/material.h"
#include "core/random.h"

class Dielectric : public Material 
{
public:
    Dielectric(F32 idx) : refractionIdx(idx) {}
    bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut) const override;

private:
    static F32 Reflectance(F32 cosine, F32 refractionIdx);
    F32 refractionIdx;
};
