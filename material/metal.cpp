#include "material/metal.h"
#include "metal.h"

// Metal
bool Metal::Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut, F32 &pdf) const
{
    Vec3 reflected = Reflect(rIn.Dir(), rec.N);
    reflected = Normalize(reflected) + (fuzz * Random::UnitVector3());
    rOut = Ray(rec.P, reflected, rIn.Time());
    attenuation = albedo;
    return (Dot(rOut.Dir(), rec.N) > 0);
}

bool Metal::Scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const
{
    Vec3 reflected = Reflect(rIn.Dir(), rec.N);
    reflected = Normalize(reflected) + (fuzz * Random::UnitVector3());

    srec.attenuation = albedo;
    srec.pdfPtr = nullptr;
    srec.isSkipPdf = true;
    srec.skipPdfRay = Ray(rec.P, reflected, rIn.Time());
    
    return true;
}
