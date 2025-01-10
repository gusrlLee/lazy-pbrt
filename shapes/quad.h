#pragma once

#include "core/hittable.h"
#include "core/hittablelist.h"

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
    virtual bool IsInterior(F32 a, F32 b, HitRecord &rec) const;

private:
    Point3 Q;
    Vec3 u, v;
    Sptr<Material> mat;
    Vec3 W;
    AABB bbox;
    Vec3 N;
    F32 D;
};

inline Sptr<HitTableList> Box(const Point3 &a, const Point3 &b, Sptr<Material> mat)
{
    auto sides = MakeSptr<HitTableList>();

    auto min = Point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = Point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = Vec3(max.x() - min.x(), 0, 0);
    auto dy = Vec3(0, max.y() - min.y(), 0);
    auto dz = Vec3(0, 0, max.z() - min.z());

    sides->Add(MakeSptr<Quad>(Point3(min.x(), min.y(), max.z()), dx, dy, mat));  // front
    sides->Add(MakeSptr<Quad>(Point3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
    sides->Add(MakeSptr<Quad>(Point3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
    sides->Add(MakeSptr<Quad>(Point3(min.x(), min.y(), min.z()), dz, dy, mat));  // left
    sides->Add(MakeSptr<Quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
    sides->Add(MakeSptr<Quad>(Point3(min.x(), min.y(), min.z()), dx, dz, mat));  // bottom

    return sides;
}