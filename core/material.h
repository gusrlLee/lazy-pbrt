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
    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rOut) const { return false; }
};