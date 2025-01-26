#ifndef GI_SHAPES_TRIANGLE_HPP
#define GI_SHAPES_TRIANGLE_HPP

#include "core/hittable.hpp"

namespace gi
{
    class Triangle : public Hittable {
        public:
            Vertex v[3];

            Triangle() {}
            Triangle(Point3 v0, Point3 v1, Point3 v2) : v{v0, v1, v2} {}
            Vertex &operator[](Int i) { return v[i]; }

            AABB BBox() const override {

            }

            Bool Hit(const Ray& r, Interval t, HitRecord &rec) const override {

            }
    };
}

#endif