#pragma once

#include "core/types.h"
#include "core/ray.h"
#include "math/vec3.h"
#include "core/random.h"

class Camera
{
public:
    void Init();
    Ray GetRay(I32 i, I32 j);

    F32 aspectRatio;
    I32 imgWidth;
    I32 spp;
    I32 maxDepth;

    F32 vFov = 90.0f;
    Point3 lookfrom = Point3(0, 0, 0);
    Point3 lookat = Point3(0, 0, -1);
    Vec3 vup = Vec3(0, 1, 0);

    F32 defocusAngle = 0;
    F32 focusDist = 10;

    F32 sppInv;

private:

    Vec3 SampleSquare() const;
    Point3 DefoucsDiskSample() const;

    int imgHeight;
    Point3 center = Point3(0, 0, 0);
    Point3 px00;

    Vec3 pxdu;
    Vec3 pxdv;

    Vec3 u, v, w;
    Vec3 defocusDiskU;
    Vec3 defocusDiskV;
};