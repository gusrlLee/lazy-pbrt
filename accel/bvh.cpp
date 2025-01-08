#include "accel/bvh.h"
#include "bvh.h"

bool BVHNode::BoxCompare(const Sptr<HitTable> a, const Sptr<HitTable> b, I32 idx)
{
    auto a_axis_interval = a->BBox().AxisInterval(idx);
    auto b_axis_interval = b->BBox().AxisInterval(idx);
    return a_axis_interval.min < b_axis_interval.min;
}

bool BVHNode::BoxCompareX(const Sptr<HitTable> a, const Sptr<HitTable> b)
{
    return BoxCompare(a, b, 0);
}

bool BVHNode::BoxCompareY(const Sptr<HitTable> a, const Sptr<HitTable> b)
{
    return BoxCompare(a, b, 1);
}

bool BVHNode::BoxCompareZ(const Sptr<HitTable> a, const Sptr<HitTable> b)
{
    return BoxCompare(a, b, 2);
}

BVHNode::BVHNode(HitTableList list) : BVHNode(list.objects, 0, list.objects.size()) {}

BVHNode::BVHNode(std::vector<Sptr<HitTable>> &objects, Size start, Size end)
{
    // I32 axis = Random::ValueI32(0, 2);

    // Build the bounding box of the span of source objects.
    bbox = AABB::empty;
    for (size_t object_index = start; object_index < end; object_index++)
        bbox = AABB(bbox, objects[object_index]->BBox());

    int axis = bbox.LongestAxis();

    // FIXME :
    // BVH의 quailty를 위해 가장 긴 축으로 설정하게 만들기
    auto comparator = (axis == 0)   ? BoxCompareX
                      : (axis == 1) ? BoxCompareY
                                    : BoxCompareZ;
    Size objectSpan = end - start;

    if (objectSpan == 1)
    {
        left = right = objects[start];
    }
    else if (objectSpan == 2)
    {
        left = objects[start];
        right = objects[start + 1];
    }
    else
    {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        auto mid = start + objectSpan / 2;
        left = MakeSptr<BVHNode>(objects, start, mid);
        right = MakeSptr<BVHNode>(objects, mid, end);
    }

    // bbox = AABB(left->BBox(), right->BBox());
}

bool BVHNode::Hit(const Ray &r, Interval t, HitRecord &rec) const
{
    if (!bbox.Hit(r, t))
        return false;

    bool isHitLeft = left->Hit(r, t, rec);
    bool isHitRight = right->Hit(r, Interval(t.min, isHitLeft ? rec.t : t.max), rec);

    return isHitLeft || isHitRight;
}
