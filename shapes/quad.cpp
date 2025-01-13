#include "shapes/quad.h"
#include "quad.h"

void Quad::SetBoundingBox()
{
    auto bboxDiagonal1 = AABB(Q, Q + u + v);
    auto bboxDiagonal2 = AABB(Q + u, Q + v);

    bbox = AABB(bboxDiagonal1, bboxDiagonal2);
}

bool Quad::Hit(const Ray& r, Interval t, HitRecord &rec) const 
{
    auto denom = Dot(N, r.Dir());
    if (std::fabs(denom) < 1e-8)
        return false;

    // intersection Point 
    auto tValue = (D - Dot(N, r.Orig())) / denom;
    if (!t.Contains(tValue))
        return false;

    auto ist = r.At(tValue);
    Vec3 planerHitPtVector = ist - Q;
    auto alpha = Dot(W, Cross(planerHitPtVector, v));
    auto beta = Dot(W, Cross(u, planerHitPtVector));

    if (!IsInterior(alpha, beta, rec))
        return false;

    rec.t = tValue;
    rec.P = ist;
    rec.mat = mat;
    rec.SetFaceNormal(r, N);
    return true;
}

bool Quad::IsInterior(F32 a, F32 b, HitRecord &rec) const
{
    Interval unitInterval = Interval(0, 1);
    if (!unitInterval.Contains(a) || !unitInterval.Contains(b))
        return false;

    rec.u = a;
    rec.v = b;
    return true;
}

F32 Quad::PdfValue(const Point3 &origin, const Vec3 &dir) const
{
    HitRecord rec;
    if (!this->Hit(Ray(origin, dir), Interval(0.001, inf), rec))
        return 0;
    
    auto distSquared = rec.t * rec.t * dir.LengthSqaured();
    auto cosine = std::fabs(Dot(dir, rec.N) / dir.Length());

    return distSquared / (cosine * area);
}
