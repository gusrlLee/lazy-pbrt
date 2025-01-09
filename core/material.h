#pragma once 

#include "core/types.h"
#include "math/vec3.h"
#include "core/ray.h"
#include "core/color.h"
#include "core/hitrecord.h"

class Material 
{
public:
    virtual ~Material() = default;
    virtual Color Emit(F32 u, F32 v, const Point3 &p) const { return Color(0, 0, 0); }
    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut) const { return false; }
};

using Light = Material;