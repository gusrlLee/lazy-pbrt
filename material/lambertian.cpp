#include "material/lambertian.h"
#include "lambertian.h"

// Lambertian
bool Lambertian::Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut, F32 &pdf) const 
{
    OrthonormalBasis onb(rec.N);

    // auto scatter_direction = rec.N + Random::();
    auto scatterDir = onb.Transform(Random::CosineDir());

    rOut = Ray(rec.P, Normalize(scatterDir), rIn.Time());
    attenuation = tex->Value(rec.u, rec.v, rec.P);
    pdf = Dot(onb.w(), rOut.Dir()) / pi;
    return true;
}

F32 Lambertian::ScatteringPdf(const Ray &rIn, const HitRecord &rec, const Ray &rOut) const
{
    return 1 / (2 * pi);
}