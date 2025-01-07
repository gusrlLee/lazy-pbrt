#pragma once 
#include "core/types.h"
#include "core/ray.h"
#include "math/vec3.h"

class Material;

class HitRecord 
{
    public:
        Point3 P;
        Vec3 N;
        F32 t;
        bool isFrontFace;
        std::shared_ptr<Material> mat;
        
        void SetFaceNormal(const Ray& r, const Vec3 &outwardNormal)
        {   
            isFrontFace = Dot(r.Dir(), outwardNormal) < 0;
            N = isFrontFace ? outwardNormal : -outwardNormal;
        }
};
