#include "core/hittablelist.h"

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
