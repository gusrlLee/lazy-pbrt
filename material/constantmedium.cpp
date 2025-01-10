#include "material/constantmedium.h"

bool ConstantMedium::Hit(const Ray &r, Interval t, HitRecord &rec) const
{
    HitRecord rec1, rec2;
    if (!boundary->Hit(r, Interval::universe, rec1))
        return false;

    if (!boundary->Hit(r, Interval(rec1.t + 0.0001, inf), rec2))
        return false;

    if (rec1.t < t.min)
        rec1.t = t.min;
    if (rec2.t > t.max)
        rec2.t = t.max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    auto rayLen = r.Dir().Length();
    auto distInsideBoundary = (rec2.t - rec1.t) * rayLen;
    auto hitDist = negInvDensity * std::log(Random::Value());

    if (hitDist > distInsideBoundary)
        return false;

    rec.t = rec1.t + hitDist / rayLen;
    rec.P = r.At(rec.t);

    rec.N = Vec3(1, 0, 0); // arbitrary
    rec.isFrontFace = true;      // also arbitrary
    rec.mat = phaseFunc;

    return true;
}