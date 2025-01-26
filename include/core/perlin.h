#ifndef GI_CORE_PERLIN_HPP
#define GI_CORE_PERLIN_HPP

#include "core/geometry.hpp"
#include "core/rng.hpp"

namespace gi
{
    class Perlin
    {
    public:
        Perlin()
        {
            for (Int i = 0; i < pointCnt; i++)
            {
                randVec[i] = Normalize(RNG::Get3D(-1, 1));
            }
            PerlinGerneratePerm(permX);
            PerlinGerneratePerm(permY);
            PerlinGerneratePerm(permZ);
        }

        Float Noise(const Point3 &p) const
        {

            auto u = p.x() - std::floor(p.x());
            auto v = p.y() - std::floor(p.y());
            auto w = p.z() - std::floor(p.z());

            auto i = Int(std::floor(p.x()));
            auto j = Int(std::floor(p.y()));
            auto k = Int(std::floor(p.z()));

            Vec3 c[2][2][2];

            for (Int di = 0; di < 2; di++)
                for (Int dj = 0; dj < 2; dj++)
                    for (Int dk = 0; dk < 2; dk++)
                        c[di][dj][dk] = randVec[permX[(i + di) & 255] ^
                                                permY[(j + dj) & 255] ^
                                                permZ[(k + dk) & 255]];

            return PerlinInterp(c, u, v, w);
        }

        Float Turb(const Point3 &p, Int depth) const
        {
            auto accum = 0.0f;
            auto tmpP = p;
            auto w = 1.0f;

            for (Int i = 0; i < depth; i++)
            {
                accum += w * Noise(tmpP);
                w *= 0.5;
                tmpP *= 2;
            }

            return std::fabs(accum);
        }

    private:
        static const Int pointCnt = 256;
        Vec3 randVec[pointCnt];
        Float randFloat[pointCnt];
        Int permX[pointCnt];
        Int permY[pointCnt];
        Int permZ[pointCnt];

        static void PerlinGerneratePerm(Int *p)
        {
            for (Int i = 0; i < pointCnt; i++)
            {
                p[i] = i;
            }
            Permute(p, pointCnt);
        }
        static void Permute(Int *p, Int n)
        {
            for (Int i = n - 1; i > 0; i--)
            {
                Int target = RNG::Get1D(0, i);
                Int temp = p[i];
                p[i] = p[target];
                p[target] = temp;
            }
        }
        static Float TriLinearInterp(Float c[2][2][2], Float u, Float v, Float w)
        {
            auto accum = 0.0;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++)
                        accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) * (k * w + (1 - k) * (1 - w)) * c[i][j][k];

            return accum;
        }
        static Float PerlinInterp(const Vec3 c[2][2][2], Float u, Float v, Float w)
        {
            auto uu = u * u * (3 - 2 * u);
            auto vv = v * v * (3 - 2 * v);
            auto ww = w * w * (3 - 2 * w);

            auto accum = 0.0;
            for (Int i = 0; i < 2; i++)
                for (Int j = 0; j < 2; j++)
                    for (Int k = 0; k < 2; k++)
                    {
                        Vec3 weightV(u - i, v - j, w - k);
                        accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * Dot(c[i][j][k], weightV);
                    }

            return accum;
        }
    };
}

#endif