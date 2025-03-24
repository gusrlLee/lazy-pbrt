#ifndef NANO_GLOBAL_ILLUMINATION_MATERIAL_HPP
#define NANO_GLOBAL_ILLUMINATION_MATERIAL_HPP

#include "nanogi/data.hpp"
#include "nanogi/random.hpp"
#include "nanogi/shape.hpp"

namespace nanogi
{
    class Pdf
    {
    public:
        virtual ~Pdf() {}
        virtual Float Value(const Vec3 &dir) const = 0;
        virtual Vec3 Gen() const = 0;
    };

    class CosinePdf : public Pdf
    {
    public:
        CosinePdf(const Vec3 &w) : uvw(w) {}
        Float Value(const Vec3 &dir) const override
        {
            auto cosTheta = Dot(Normalize(dir), uvw.w());
            return std::fmax(0, cosTheta / Pi);
        }
        Vec3 Gen() const override
        {
            return uvw.Transform(Random::CosineDirection());
        }

    private:
        Onb uvw;
    };

    class SurfacePdf : public Pdf
    {
    public:
        SurfacePdf(const Triangle &objects, const Point3 &origin)
            : objects(objects), origin(origin) {}

        Float Value(const Vec3 &dir) const override
        {
            return objects.PdfValue(Ray(origin, dir));
        }

        Vec3 Gen() const override
        {
            return objects.Sample(origin);
        }

    private:
        const Triangle &objects;
        Point3 origin;
    };

    class LightPdf : public Pdf
    {
    public:
        LightPdf(const std::vector<Triangle> &list, const Point3 &origin) : list(list), origin(origin) {}
        Float Value(const Vec3 &dir) const override
        {
            auto weight = 1.0 / list.size();
            auto sum = 0.0f;
            
            for (const auto &element : list) {
                sum += weight * element.PdfValue(Ray(origin, dir));
            }

            return sum;
        }
        

        // FIXME: 
        // If list size is zero, maybe this function cannot be return.
        // So, we have to modify this part to return.
        Vec3 Gen() const override
        {
            auto listSize = Int(list.size());

            // to avoid segementation fault
            while (true)
            {
                Int idx = Int(Random::Get1D(0, listSize));
                if (idx <= listSize - 1)
                    return list[idx].Sample(origin);
            }
        }
    private:
        const std::vector<Triangle> &list;
        Point3 origin;
    };

    class MixPdf : public Pdf
    {
    public:
        MixPdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1) : p{p0, p1} {}

        Float Value(const Vec3 &dir) const override
        {
            // auto p0 = p[0]->Value(dir) / (p[0]->Value(dir) + p[1]->Value(dir));
            // auto p1 = p[1]->Value(dir) / (p[0]->Value(dir) + p[1]->Value(dir));
            return 0.5 * p[0]->Value(dir) + 0.5 * p[1]->Value(dir);
        }

        Vec3 Gen() const override
        {
            if (Random::Get1D() < 0.5)
                return p[0]->Gen();
            else
                return p[1]->Gen();
        }

    private:
        std::shared_ptr<Pdf> p[2];
    };

    class Material
    {
    public:
        ~Material() = default;

        virtual bool IsLight() { return false; }
        virtual Color Le(const Ray &ray, const Payload &payload) { return Color(0.0); }

        virtual Vec3 F(const Ray &wi, Payload &payload) { return Vec3(0.0); }
        virtual Float Pdf(const Ray &wi, const Payload &payload) { return 0; }
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(Color albedo) : albedo(albedo) {}
        Vec3 F(const Ray &wi, Payload &payload) override
        {
            // Onb uvw(payload.hRec.n);
            // auto outDir = Normalize(uvw.Transform(Random::CosineDirection()));
            // Ray outgoing = Ray(payload.hRec.p, outDir);
            // payload.sRec.nextRay = outgoing;
            payload.sRec.pPdf = std::make_shared<CosinePdf>(payload.hRec.n);
            return albedo;
        }

        Float Pdf(const Ray &wi, const Payload &payload) override
        {
            auto cosTheta = Dot(payload.hRec.n, Normalize(payload.sRec.nextRay.Dir()));
            return cosTheta < 0 ? 0 : cosTheta / Pi;
        }

    private:
        Color albedo;
    };

    class DiffuseLight : public Material
    {
    public:
        DiffuseLight(Vec3 intensity) : intensity(intensity) {}

        bool IsLight() override { return true; }
        Color Le(const Ray &ray, const Payload &payload) override
        {
            bool isSetFront = Dot(ray.Dir(), payload.hRec.n) < 0;
            if (!isSetFront)
                return Color(0.0);
            return intensity;
        }

    private:
        Vec3 intensity;
    };

} // namespace nanogi

#endif