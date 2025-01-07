#include "material/lambertian.h"

// Lambertian
bool Lambertian::Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut) const 
{
    auto scatter_direction = rec.N + Random::UnitVector3();
    // Catch degenerate scatter direction 
    if (scatter_direction.IsNearZero())
        scatter_direction = rec.N;

    rOut = Ray(rec.P, scatter_direction, rIn.Time());
    attenuation = albedo;
    return true;
}
