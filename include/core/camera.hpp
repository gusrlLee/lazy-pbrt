#ifndef GI_CORE_CAMERA_HPP
#define GI_CORE_CAMERA_HPP

#include "core/geometry.hpp"
#include "core/rng.hpp"

namespace gi
{
    class Camera
    {
    public:
        void Init()
        {
            // Calculate the image height, and ensure that it's at least.
            imgHeight = Int(imgWidth / aspectRatio);
            imgHeight = (imgHeight < 1) ? 1 : imgHeight;

            // Camera center point
            center = lookfrom;
            sqrtSpp = Int(std::sqrt(spp));
            sppInv = 1.0 / (sqrtSpp * sqrtSpp);
            recipSqrtSpp = 1.0 / sqrtSpp;

            // Camera settnig
            auto theta = DegreeToRadians(vFov);
            auto h = std::tan(theta / 2);

            auto vport_h = 2 * h * focusDist;
            auto vport_w = vport_h * (Float(imgWidth) / imgHeight);

            // Calculate the u, v, w unit basis vectors for the caemra coordinate frame.
            w = Normalize(lookfrom - lookat);
            u = Normalize(Cross(vup, w));
            v = Cross(w, u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto vportU = vport_w * u;
            auto vportV = vport_h * -v;

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pxdu = vportU / Float(imgWidth);
            pxdv = vportV / Float(imgHeight);

            // Calculate the location of the upper left pixel
            auto vport_upper_left = center - (focusDist * w) - vportU / 2 - vportV / 2;
            px00 = vport_upper_left + 0.5f * (pxdu + pxdv);

            auto defocusRadius = focusDist * std::tan(DegreeToRadians(defocusAngle / 2));
            defocusDiskU = u * defocusRadius;
            defocusDiskV = v * defocusRadius;
        }

        Ray GetRay(Int i, Int j, Int si, Int sj) {
            auto offset = SampleSquareStratified(si, sj);
            auto px_sample = px00 + ((i + offset.x()) * pxdu) + ((j + offset.y()) * pxdv);

            auto ray_o = (defocusAngle <= 0) ? center : DefoucsDiskSample();
            auto ray_d = px_sample - ray_o;
            auto ray_tm = RNG::Get1D();

            return Ray(ray_o, ray_d, ray_tm);
        }

        Int GetWidth() { return imgWidth; }
        Int GetHeight() { return imgHeight; }

        Float aspectRatio;
        Int imgWidth;
        Int spp;

        Int sqrtSpp;
        Float recipSqrtSpp;

        Int maxDepth;

        Float vFov = 90.0f;
        Point3 lookfrom = Point3(0, 0, 0);
        Point3 lookat = Point3(0, 0, -1);
        Vec3 vup = Vec3(0, 1, 0);

        Float defocusAngle = 0;
        Float focusDist = 10;

        Float sppInv;

    private:
        Vec3 SampleSquare() const
        {
            return Vec3(RNG::Get1D() - 0.5, RNG::Get1D() - 0.5, 0);
        }

        Vec3 SampleSquareStratified(Int si, Int sj) const
        {
            auto px = ((Float(si) + RNG::Get1D()) * recipSqrtSpp) - 0.5;
            auto py = ((Float(sj) + RNG::Get1D()) * recipSqrtSpp) - 0.5;

            return Vec3(px, py, 0);
        }

        Point3 DefoucsDiskSample() const
        {
            auto p = RNG::GetInUnitDisk();
            return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
        }

        int imgHeight;
        Point3 center = Point3(0, 0, 0);
        Point3 px00;

        Vec3 pxdu;
        Vec3 pxdv;

        Vec3 u, v, w;
        Vec3 defocusDiskU;
        Vec3 defocusDiskV;
    };

}

#endif