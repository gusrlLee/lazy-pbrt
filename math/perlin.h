#pragma once
#include "core/types.h"
#include "core/random.h"
#include "math/vec3.h"

class Perlin
{
public:
    Perlin();
    F32 Noise(const Point3 &p) const;
    F32 Turb(const Point3 &p, I32 depth) const;

private:
    static const I32 pointCnt = 256;
    Vec3 randVec[pointCnt];
    F32 randFloat[pointCnt];
    I32 permX[pointCnt];
    I32 permY[pointCnt];
    I32 permZ[pointCnt];

    static void PerlinGerneratePerm(I32 *p);
    static void Permute(I32 *p, I32 n);
    static F32 TriLinearInterp(F32 c[2][2][2], F32 u, F32 v, F32 w);
    static F32 PerlinInterp(const Vec3 c[2][2][2], F32 u, F32 v, F32 w);

};