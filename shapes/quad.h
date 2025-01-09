#pragma once 

#include "core/hittable.h"

class Quad : public HitTable
{
public:
    Quad(const Point3 &Q, const Vec3 &u, const Vec3 v, Sptr<Material> mat) : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = Cross(u, v);
        this->N = Normalize(n);
        D = Dot(N, Q);
        W = n / Dot(n, n);

        SetBoundingBox();
    }

    virtual void SetBoundingBox();

    AABB BBox() const override { return bbox; }
    bool Hit(const Ray &r, Interval t, HitRecord &rec) const override;
    virtual bool IsInterior(F32 a, F32 b, HitRecord& rec) const;

private:
    Point3 Q;
    Vec3 u, v;
    Sptr<Material> mat;
    Vec3 W;
    AABB bbox;
    Vec3 N;
    F32 D;
};
