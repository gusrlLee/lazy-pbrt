#ifndef GI_CORE_MATERIALS_HPP
#define GI_CORE_MATERIALS_HPP

#include "core/geometry.hpp"
#include "core/sample.hpp"

namespace gi
{
    class Material
    {
    public:
        virtual ~Material() = default;
        virtual Bool Scatter(const Ray &ray, const RayPayload &payload, Vec3f &attenuation, Ray &outRay) const { return false; }
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(const Vec3f &albedo) : albedo(albedo) {}
        Bool Scatter(const Ray &ray, const RayPayload &payload, Vec3f &attenuation, Ray &outRay) const override
        {
            auto rDir = Add(payload.n, Sample::Direction());
            outRay = Ray{payload.p, rDir};
            attenuation = albedo;
            return true;
        }

    private:
        Vec3f albedo;
    };
} // namespace gi
#endif