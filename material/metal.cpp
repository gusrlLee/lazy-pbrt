#include "material/metal.h"

// Metal
bool Metal::Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut) const
{
    Vec3 reflected = Reflect(rIn.Dir(), rec.N);
    reflected = Normalize(reflected) + (fuzz * Random::UnitVector3());
    rOut = Ray(rec.P, reflected, rIn.Time());
    attenuation = albedo;
    return (Dot(rOut.Dir(), rec.N) > 0);
}
