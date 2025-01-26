#ifndef GI_CORE_PDF_HPP
#define GI_CORE_PDF_HPP

#include "core/hittable.hpp"
#include "core/onb.hpp"
#include "core/rng.hpp"

namespace gi
{
    class Pdf
    {
    public:
        virtual ~Pdf() {}

        virtual Float Value(const Vec3 &dir) const = 0;
        virtual Vec3 Gen() const = 0;
    };

    class SpherePdf : public Pdf
    {
    public:
        SpherePdf() {}

        Float Value(const Vec3 &dir) const override
        {
            return 1 / (4 * PI);
        }

        Vec3 Gen() const override
        {
            return RNG::GetUnit3D();
        }
    };

    class CosPdf : public Pdf
    {
    public:
        CosPdf(const Vec3 &w) : uvw(w) {}
        Float Value(const Vec3 &dir) const override
        {
            auto cosTheta = Dot(Normalize(dir), uvw.w());
            return std::fmax(0, cosTheta / PI);
        }

        Vec3 Gen() const override
        {
            return uvw.Transform(RNG::GetCosDir());
        }

    private:
        ONB uvw;
    };

    class HittablePdf : public Pdf
    {
    public:
        HittablePdf(const Hittable &objects, const Point3 &origin) : objects(objects), origin(origin) {}

        Float Value(const Vec3 &dir) const override
        {
            return objects.PdfValue(origin, dir);
        }

        Vec3 Gen() const override
        {
            return objects.Random(origin);
        }

    private:
        const Hittable &objects;
        Point3 origin;
    };

    class MixPdf : public Pdf
    {
    public:
        MixPdf(SPtr<Pdf> p0, SPtr<Pdf> p1)
        {
            p[0] = p0;
            p[1] = p1;
        }

        Float Value(const Vec3 &dir) const override
        {
            return 0.5 * p[0]->Value(dir) + 0.5 * p[1]->Value(dir);
        }

        Vec3 Gen() const override
        {
            if (RNG::Get1D() < 0.5)
                return p[0]->Gen();
            else
                return p[1]->Gen();
        }

    private:
        SPtr<Pdf> p[2];
    };
}

#endif