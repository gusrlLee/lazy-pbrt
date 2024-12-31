#include "material.h"

// Lambertian
bool Lambertian::scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const 
{
    auto scatter_direction = rec.N + random_unit_vector();
    // Catch degenerate scatter direction 
    if (scatter_direction.near_zero())
        scatter_direction = rec.N;

    scattered = Ray(rec.P, scatter_direction);
    attenuation = albedo;
    return true;
}

// Metal
bool Metal::scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const 
{
    Vec3 reflected = reflect(r_in.direction(), rec.N);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = Ray(rec.P, reflected);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.N) > 0);
}

// Dielectric
f64 Dielectric::reflectance(f64 cosine, f64 refraction_index)
{
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

bool Dielectric::scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const 
{
    attenuation = Color(1.0, 1.0, 1.0);
    f64 ri = rec.front_face ? (1.0 / refraction_idx) : refraction_idx;

    Vec3 unit_direction = unit_vector(r_in.direction());
    f64 cos_theta = std::fmin(dot(-unit_direction, rec.N), 1.0);
    f64 sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    Vec3 direction;
    if (cannot_refract || reflectance(cos_theta, ri) > random_value())
        direction = reflect(unit_direction, rec.N);
    else 
        direction = refract(unit_direction, rec.N, ri);

    scattered = Ray(rec.P, direction);
    return true;
}
