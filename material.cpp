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
