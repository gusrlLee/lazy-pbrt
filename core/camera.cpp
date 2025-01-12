#include "core/camera.h"
#include "camera.h"

void Camera::Init()
{
    // Calculate the image height, and ensure that it's at least.
    imgHeight = I32(imgWidth / aspectRatio);
    imgHeight = (imgHeight < 1) ? 1 : imgHeight;

    // Camera center point
    center = lookfrom;
    sqrtSpp = I32(std::sqrt(spp));
    sppInv = 1.0 / (sqrtSpp * sqrtSpp);
    recipSqrtSpp = 1.0 / sqrtSpp;
    
    // Camera settnig
    auto theta = DegreeToRadians(vFov);
    auto h = std::tan(theta / 2);

    auto vport_h = 2 * h * focusDist;
    auto vport_w = vport_h * (F32(imgWidth) / imgHeight);

    // Calculate the u, v, w unit basis vectors for the caemra coordinate frame.
    w = Normalize(lookfrom - lookat);
    u = Normalize(Cross(vup, w));
    v = Cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto vportU = vport_w * u;
    auto vportV = vport_h * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pxdu = vportU / F32(imgWidth);
    pxdv = vportV / F32(imgHeight);
    
    // Calculate the location of the upper left pixel
    auto vport_upper_left = center - (focusDist * w) - vportU / 2 - vportV / 2;
    px00 = vport_upper_left + 0.5f * (pxdu + pxdv);

    auto defocusRadius = focusDist * std::tan(DegreeToRadians(defocusAngle / 2));
    defocusDiskU = u * defocusRadius;
    defocusDiskV = v * defocusRadius;
}

Ray Camera::GetRay(I32 i, I32 j, I32 si, I32 sj)
{
    // Construct a camera ray originating from the defocus disk and directed at a randomly 
    // sampled point around the pixel location i, j.
    // auto offset = SampleSquare();

    auto offset = SampleSquareStratified(si, sj);
    auto px_sample = px00 + ((i + offset.x()) * pxdu) + ((j + offset.y()) * pxdv);

    auto ray_o = (defocusAngle <= 0) ? center : DefoucsDiskSample();
    auto ray_d = px_sample - ray_o;
    auto ray_tm = Random::Value();

    return Ray(ray_o, ray_d, ray_tm);
}

Vec3 Camera::SampleSquare() const
{
    return Vec3(Random::Value() - 0.5, Random::Value() - 0.5, 0);
}

Vec3 Camera::SampleSquareStratified(I32 si, I32 sj) const
{
    auto px = ((F32(si) + Random::Value()) * recipSqrtSpp) - 0.5; 
    auto py = ((F32(sj) + Random::Value()) * recipSqrtSpp) - 0.5; 

    return Vec3(px, py, 0);
}

Point3 Camera::DefoucsDiskSample() const
{
    auto p = Random::InUnitDisk();
    return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
}