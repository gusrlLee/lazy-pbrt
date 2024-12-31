#pragma once 
#include "types.h"
#include "primitive.h"
#include "vec3.h"
#include "ray.h"
#include "hit_record.h"
#include "color.h"

class Material 
{
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const { return false; }
};

class Lambertian : public Material 
{
public:
    Lambertian(const Color& albedo) : albedo(albedo) {}
    bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;

private:
    Color albedo;
};

class Metal : public Material 
{
public:
    Metal(const Color& albedo, f64 fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;

private:
    Color albedo;
    f64 fuzz;
};

