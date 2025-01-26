#ifndef GI_ACCELERATION_STRUCTURES_BVH_HPP
#define GI_ACCELERATION_STRUCTURES_BVH_HPP

#include "core/hittablelist.hpp"

namespace gi
{
    class BVHNode : public Hittable
    {
    public:
        BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {}
        BVHNode(std::vector<SPtr<Hittable>> &objects, Size start, Size end)
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
                left = MakeShared<BVHNode>(objects, start, mid);
                right = MakeShared<BVHNode>(objects, mid, end);
            }

            // bbox = AABB(left->BBox(), right->BBox());
        }

        bool Hit(const Ray &r, Interval t, HitRecord &rec) const override
        {
            if (!bbox.Hit(r, t))
                return false;

            bool isHitLeft = left->Hit(r, t, rec);
            bool isHitRight = right->Hit(r, Interval(t.min, isHitLeft ? rec.t : t.max), rec);

            return isHitLeft || isHitRight;
        }

        AABB BBox() const override { return bbox; }

    private:
        SPtr<Hittable> left;
        SPtr<Hittable> right;
        AABB bbox;

        static bool BoxCompare(const SPtr<Hittable> a, const SPtr<Hittable> b, Int idx)
        {
            auto a_axis_interval = a->BBox().AxisInterval(idx);
            auto b_axis_interval = b->BBox().AxisInterval(idx);
            return a_axis_interval.min < b_axis_interval.min;
        }
        static bool BoxCompareX(const SPtr<Hittable> a, const SPtr<Hittable> b)
        {
            return BoxCompare(a, b, 0);
        }
        static bool BoxCompareY(const SPtr<Hittable> a, const SPtr<Hittable> b)
        {
            return BoxCompare(a, b, 1);
        }
        static bool BoxCompareZ(const SPtr<Hittable> a, const SPtr<Hittable> b)
        {
            return BoxCompare(a, b, 2);
        }
    };
}

#endif