#pragma once

#include "core/hittable.h"
#include "core/material.h"
#include "core/texture.h"
#include "material/isotropic.h"

class ConstantMedium : public HitTable
{
public:
    ConstantMedium(Sptr<HitTable> boundary, F32 density, Sptr<Texture> tex) :
        boundary(boundary),
        negInvDensity(-1 / density),
        phaseFunc(MakeSptr<Isotropic>(tex)) {}
    
    ConstantMedium(Sptr<HitTable> boundary, F32 density, const Color &albedo) :
        boundary(boundary),
        negInvDensity(-1 / density),
        phaseFunc(MakeSptr<Isotropic>(albedo)) {}

    bool Hit(const Ray &r, Interval t, HitRecord &rec) const override;

    AABB BBox() const override { return boundary->BBox(); } 

private:
    Sptr<HitTable> boundary;
    F32 negInvDensity;
    Sptr<Material> phaseFunc;
};