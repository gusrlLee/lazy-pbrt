#pragma once 
#include "math/vec3.h"

// ONB
class OrthonormalBasis 
{
public:
    OrthonormalBasis(const Vec3 &n) 
    {
        axis[2] = Normalize(n);
        Vec3 a = (std::fabs(axis[2].x()) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
        axis[1] = Normalize(Cross(axis[2], a));
        axis[0] = Cross(axis[2], axis[1]);
    }

    const Vec3& u() const { return axis[0]; }
    const Vec3& v() const { return axis[1]; }
    const Vec3& w() const { return axis[2]; }

    Vec3 Transform(const Vec3 &v) const 
    {
        return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
    }

private:
    Vec3 axis[3];
};