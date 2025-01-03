#pragma once 
#include "types.h"
#include "vec3.h"
#include "ray.h"

class Material;

class hit_record 
{
    public:
        Point3 P;
        Vec3 N;
        double t;
        bool front_face;
        std::shared_ptr<Material> mat;
        
        void set_face_normal(const Ray& r, const Vec3 &outward_normal)
        {   
            front_face = dot(r.direction(), outward_normal) < 0;
            N = front_face ? outward_normal : -outward_normal;
        }
};
