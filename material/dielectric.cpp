#include "material/dielectric.h"

// Dielectric
F32 Dielectric::Reflectance(F32 cosine, F32 refractionIdx)
{
    auto r0 = (1 - refractionIdx) / (1 + refractionIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

bool Dielectric::Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut) const 
{
    attenuation = Color(1.0, 1.0, 1.0);
    F32 ri = rec.isFrontFace ? (1.0 / refractionIdx) : refractionIdx;

    Vec3 unitDir = Normalize(rIn.Dir());
    F32 cosTheta = std::fmin(Dot(-unitDir, rec.N), 1.0);
    F32 sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool isCannotRefract = ri * sinTheta > 1.0;
    Vec3 direction;
    if (isCannotRefract || Reflectance(cosTheta, ri) > Random::Value())
        direction = Reflect(unitDir, rec.N);
    else 
        direction = Refract(unitDir, rec.N, ri);

    rOut = Ray(rec.P, direction, rIn.Time());
    return true;
}
