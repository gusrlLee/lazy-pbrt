#ifndef GI_CORE_HITRECORD_HPP
#define GI_CORE_HITRECORD_HPP
#include "core/geometry.hpp"
namespace gi
{
    class Material;
    class HitRecord
    {
    public:
        Point3 P;
        Vec3 N;
        Float t;
        Float u;
        Float v;
        Bool isFrontFace;

        SPtr<Material> mat;

        void SetFaceNormal(const Ray &r, const Vec3 &outwardNormal)
        {
            isFrontFace = Dot(r.Dir(), outwardNormal) < 0;
            N = isFrontFace ? outwardNormal : -outwardNormal;
        }
    };

}
#endif