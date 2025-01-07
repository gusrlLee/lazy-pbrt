#pragma once

#include "math/vec3.h"

#include "core/material.h"
#include "core/color.h"

#include "core/random.h"

class Lambertian : public Material
{
public:
    Lambertian(const Color &albedo) : albedo(albedo) {}
    bool Scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation, Ray &rOut) const override;

private:
    Color albedo;
};