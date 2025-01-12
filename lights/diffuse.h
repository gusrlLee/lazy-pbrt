#pragma once

#include "core/light.h"
#include "core/texture.h"

class DiffuseLight : public Light
{
public:
    DiffuseLight(Sptr<Texture> tex) : tex(tex) {}
    DiffuseLight(const Color &emit) : tex(MakeSptr<SolidColor>(emit)) {}

    Color Emit(const Ray &rIn, const HitRecord &rec, F32 u, F32 v, const Point3 &p) const override
    {
        if (!rec.isFrontFace)
            return Color(0, 0, 0);
        return tex->Value(u, v, p);
    }

private:
    Sptr<Texture> tex;
};