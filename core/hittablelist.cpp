#include "core/hittablelist.h"
#include "hittablelist.h"

bool HitTableList::Hit(const Ray &r, Interval t, HitRecord &rec) const
{
    HitRecord tempRec;
    bool isHitAnything = false;
    auto closestSoFar = t.max;

    for (const auto &object : objects)
    {
        // execute to intersection test about current primitive
        if (object->Hit(r, Interval(t.min, closestSoFar), tempRec))
        {
            isHitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return isHitAnything;
}

F32 HitTableList::PdfValue(const Point3 &origin, const Vec3 &dir) const
{
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
    {
        sum += weight * object->PdfValue(origin, dir);
    }

    return sum;
}
