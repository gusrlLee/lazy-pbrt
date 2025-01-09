#include "math/perlin.h"
#include "perlin.h"

Perlin::Perlin()
{
    for (I32 i = 0; i < pointCnt; i++)
    {
        randVec[i] = Normalize(Random::Vector3(-1, 1));
    }
    PerlinGerneratePerm(permX);
    PerlinGerneratePerm(permY);
    PerlinGerneratePerm(permZ);
}

F32 Perlin::Noise(const Point3 &p) const
{

    auto u = p.x() - std::floor(p.x());
    auto v = p.y() - std::floor(p.y());
    auto w = p.z() - std::floor(p.z());

    auto i = I32(std::floor(p.x()));
    auto j = I32(std::floor(p.y()));
    auto k = I32(std::floor(p.z()));

    Vec3 c[2][2][2];

    for (I32 di = 0; di < 2; di++)
        for (I32 dj = 0; dj < 2; dj++)
            for (I32 dk = 0; dk < 2; dk++)
                c[di][dj][dk] = randVec[permX[(i + di) & 255] ^
                                        permY[(j + dj) & 255] ^
                                        permZ[(k + dk) & 255]];

    return PerlinInterp(c, u, v, w);
}

F32 Perlin::Turb(const Point3 &p, I32 depth) const
{
    auto accum = 0.0f;
    auto tmpP = p;
    auto w = 1.0f;

    for (I32 i = 0; i < depth; i++)
    {
        accum += w * Noise(tmpP);
        w *= 0.5;
        tmpP *= 2;
    }

    return std::fabs(accum);
}

void Perlin::PerlinGerneratePerm(I32 *p)
{
    for (I32 i = 0; i < pointCnt; i++)
    {
        p[i] = i;
    }
    Permute(p, pointCnt);
}

void Perlin::Permute(I32 *p, I32 n)
{
    for (I32 i = n - 1; i > 0; i--)
    {
        I32 target = Random::ValueI32(0, i);
        I32 temp = p[i];
        p[i] = p[target];
        p[target] = temp;
    }
}

F32 Perlin::TriLinearInterp(F32 c[2][2][2], F32 u, F32 v, F32 w)
{
    auto accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) * (k * w + (1 - k) * (1 - w)) * c[i][j][k];

    return accum;
}
F32 Perlin::PerlinInterp(const Vec3 c[2][2][2], F32 u, F32 v, F32 w)
{
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);

    auto accum = 0.0;
    for (I32 i = 0; i < 2; i++)
        for (I32 j = 0; j < 2; j++)
            for (I32 k = 0; k < 2; k++)
            {
                Vec3 weightV(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * Dot(c[i][j][k], weightV);
            }

    return accum;
}