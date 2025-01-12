#pragma once 
#include "core/random.h"
#include "core/material.h"

class Metal : public Material 
{
public:
    Metal(const Color& albedo, F32 fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut, F32 &pdf) const override;

private:
    Color albedo;
    F32 fuzz;
};