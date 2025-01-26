#ifndef GI_CORE_MATERIAL_HPP
#define GI_CORE_MATERIAL_HPP

#include "core/hittable.hpp"
#include "core/pdf.hpp"
#include "core/texture.hpp"

namespace gi
{
    class ScatterRecord
    {
    public:
        Color attenuation;
        SPtr<Pdf> pdfPtr;
        bool isSkipPdf;
        Ray skipPdfRay;
    };

    class Material
    {
    public:
        virtual ~Material() = default;
        virtual Color Emit(const Ray &rIn, const HitRecord &rec, Float u, Float v, const Point3 &p) const { return Color(0, 0, 0); }
        virtual bool Scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation, Ray &rOut, Float &pdf) const { return false; }
        virtual bool Scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const { return false; }

        virtual Float ScatteringPdf(const Ray &rIn, const HitRecord &rec, const Ray &rOut) const { return 0; }
    };
    class Metal : public Material
    {
    public:
        Metal(const Color &albedo, Float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
        bool Scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation, Ray &rOut, Float &pdf) const override
        {
            Vec3 reflected = Reflect(rIn.Dir(), rec.N);
            reflected = Normalize(reflected) + (fuzz * RNG::GetUnit3D());
            rOut = Ray(rec.P, reflected, rIn.Time());
            attenuation = albedo;
            return (Dot(rOut.Dir(), rec.N) > 0);
        }
        bool Scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const override
        {
            Vec3 reflected = Reflect(rIn.Dir(), rec.N);
            reflected = Normalize(reflected) + (fuzz * RNG::GetUnit3D());

            srec.attenuation = albedo;
            srec.pdfPtr = nullptr;
            srec.isSkipPdf = true;
            srec.skipPdfRay = Ray(rec.P, reflected, rIn.Time());

            return true;
        }

    private:
        Color albedo;
        Float fuzz;
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(const Color &albedo) : tex(MakeShared<SolidColor>(albedo)) {}
        Lambertian(SPtr<Texture> tex) : tex(tex) {}

        bool Scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation, Ray &rOut, Float &pdf) const override
        {
            ONB onb(rec.N);

            // auto scatter_direction = rec.N + Random::();
            auto scatterDir = onb.Transform(RNG::GetCosDir());

            rOut = Ray(rec.P, Normalize(scatterDir), rIn.Time());
            attenuation = tex->Value(rec.u, rec.v, rec.P);
            pdf = Dot(onb.w(), rOut.Dir()) / PI;
            return true;
        }
        bool Scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const override
        {
            srec.attenuation = tex->Value(rec.u, rec.v, rec.P);
            srec.pdfPtr = MakeShared<CosPdf>(rec.N);
            srec.isSkipPdf = false;
            return true;
        }

        Float ScatteringPdf(const Ray &rIn, const HitRecord &rec, const Ray &rOut) const override
        {
            auto cosTheta = Dot(rec.N, Normalize(rOut.Dir()));
            return cosTheta < 0 ? 0 : cosTheta / PI;
        }

    private:
        // Color albedo;
        SPtr<Texture> tex;
    };

    class Isotropic : public Material
    {
    public:
        Isotropic(const Color &albedo) : tex(MakeShared<SolidColor>(albedo)) {}
        Isotropic(SPtr<Texture> tex) : tex(tex) {}

        bool Scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation, Ray &rOut, Float &pdf) const override
        {
            rOut = Ray(rec.P, RNG::GetUnit3D(), rIn.Time());
            attenuation = tex->Value(rec.u, rec.v, rec.P);
            pdf = 1 / (4 * PI);
            return true;
        }

        bool Scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const override
        {
            srec.attenuation = tex->Value(rec.u, rec.v, rec.P);
            srec.pdfPtr = MakeShared<SpherePdf>();
            srec.isSkipPdf = false;
            return true;
        }

        Float ScatteringPdf(const Ray &rIn, const HitRecord &rec, const Ray &rOut) const override
        {
            return 1 / (4 * PI);
        }

    private:
        SPtr<Texture> tex;
    };

    class Dielectric : public Material
    {
    public:
        Dielectric(Float idx) : refractionIdx(idx) {}
        bool Scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation, Ray &rOut, Float &pdf) const override
        {
            attenuation = Color(1.0, 1.0, 1.0);
            Float ri = rec.isFrontFace ? (1.0 / refractionIdx) : refractionIdx;

            Vec3 unitDir = Normalize(rIn.Dir());
            Float cosTheta = std::fmin(Dot(-unitDir, rec.N), 1.0);
            Float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

            bool isCannotRefract = ri * sinTheta > 1.0;
            Vec3 direction;
            if (isCannotRefract || Reflectance(cosTheta, ri) > RNG::Get1D())
                direction = Reflect(unitDir, rec.N);
            else
                direction = Refract(unitDir, rec.N, ri);

            rOut = Ray(rec.P, direction, rIn.Time());
            return true;
        }

        bool Scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const override
        {
            srec.attenuation = Color(1.0, 1.0, 1.0);
            srec.pdfPtr = nullptr;
            srec.isSkipPdf = true;

            Float ri = rec.isFrontFace ? (1.0 / refractionIdx) : refractionIdx;

            Vec3 unitDir = Normalize(rIn.Dir());
            Float cosTheta = std::fmin(Dot(-unitDir, rec.N), 1.0);
            Float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

            bool isCannotRefract = ri * sinTheta > 1.0;
            Vec3 direction;
            if (isCannotRefract || Reflectance(cosTheta, ri) > RNG::Get1D())
                direction = Reflect(unitDir, rec.N);
            else
                direction = Refract(unitDir, rec.N, ri);

            srec.skipPdfRay = Ray(rec.P, direction, rIn.Time());
            return true;
        }

    private:
        static Float Reflectance(Float cosine, Float refractionIdx)
        {
            auto r0 = (1 - refractionIdx) / (1 + refractionIdx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }

        Float refractionIdx;
    };

    class DiffuseLight : public Material
    {
    public:
        DiffuseLight(SPtr<Texture> tex) : tex(tex) {}
        DiffuseLight(const Color &emit) : tex(MakeShared<SolidColor>(emit)) {}

        Color Emit(const Ray &rIn, const HitRecord &rec, Float u, Float v, const Point3 &p) const override
        {
            if (!rec.isFrontFace)
                return Color(0, 0, 0);
            return tex->Value(u, v, p);
        }

    private:
        SPtr<Texture> tex;
    };

}

#endif