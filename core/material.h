#pragma once 

#include "core/types.h"
#include "math/vec3.h"
#include "core/ray.h"
#include "core/color.h"
#include "core/hitrecord.h"

#include "core/onb.h"

class Material 
{
public:
    virtual ~Material() = default;
    virtual Color Emit(const Ray &rIn, const HitRecord &rec, F32 u, F32 v, const Point3 &p) const { return Color(0, 0, 0); }
    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut, F32 &pdf) const { return false; }

    virtual F32 ScatteringPdf(const Ray& rIn, const HitRecord &rec, const Ray &rOut) const { return 0; }
};

using Light = Material;