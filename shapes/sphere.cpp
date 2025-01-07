#include "shapes/sphere.h"

bool Sphere::Hit(const Ray &r, Interval t, HitRecord &rec) const 
{
    Point3 currCenter = center.At(r.Time());
    Vec3 oc = currCenter - r.Orig();
    auto a = r.Dir().LengthSqaured();
    auto h = Dot(r.Dir(), oc);
    auto c = oc.LengthSqaured() - radius * radius;

    auto d = h * h - a * c;
    if (d < 0)
        return false;

    auto sqrtd = sqrt(d);
    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!t.Surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!t.Surrounds(root))
            return false;
    }

    rec.t = root;
    rec.P = r.At(rec.t);
    Vec3 outNormal = (rec.P - currCenter) / radius;
    rec.SetFaceNormal(r, outNormal);
    rec.mat = mat;

    return true;
}
