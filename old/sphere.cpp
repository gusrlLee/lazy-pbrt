#include "sphere.h"

bool Sphere::hit(const Ray &r, Interval r_t, hit_record &rec) const 
{
    Point3 current_center = center.at(r.time());
    Vec3 oc = current_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto d = h * h - a * c;
    if (d < 0)
        return false;

    auto sqrtd = sqrt(d);
    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!r_t.surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!r_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.P = r.at(rec.t);
    Vec3 outward_normal = (rec.P - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
}
