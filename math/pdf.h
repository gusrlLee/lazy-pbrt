#pragma once

#include "core/onb.h"
#include "core/random.h"
#include "core/hittablelist.h"

class Pdf
{
public:
    virtual ~Pdf() {}

    virtual F32 Value(const Vec3 &dir) const = 0;
    virtual Vec3 Gen() const = 0;
};

class SpherePdf : public Pdf
{
public:
    SpherePdf() {}

    F32 Value(const Vec3 &dir) const override
    {
        return 1 / (4 * pi);
    }

    Vec3 Gen() const override
    {
        return Random::UnitVector3();
    }
};

class CosPdf : public Pdf
{
public:
    CosPdf(const Vec3 &w) : uvw(w) {}
    F32 Value(const Vec3 &dir) const override
    {
        auto cosTheta = Dot(Normalize(dir), uvw.w());
        return std::fmax(0, cosTheta / pi);
    }

    Vec3 Gen() const override
    {
        return uvw.Transform(Random::CosineDir());
    }

private:
    OrthonormalBasis uvw;
};

class HitTablePdf : public Pdf
{
public:
    HitTablePdf(const HitTable &objects, const Point3 &origin) : objects(objects), origin(origin){}

    F32 Value(const Vec3 &dir) const override 
    {
        return objects.PdfValue(origin, dir);
    }

    Vec3 Gen() const override 
    {
        return objects.Random(origin);
    }

private:
    const HitTable &objects;
    Point3 origin;
};

class MixPdf : public Pdf
{
public:
    MixPdf(Sptr<Pdf> p0, Sptr<Pdf> p1) 
    {
        p[0] = p0;
        p[1] = p1;
    }

    F32 Value(const Vec3 &dir) const override 
    {
        return 0.5 * p[0]->Value(dir) + 0.5 * p[1]->Value(dir);
    }

    Vec3 Gen() const override
    {
        if (Random::Value() < 0.5)
            return p[0]->Gen();
        else 
            return p[1]->Gen();
    }

private:
    Sptr<Pdf> p[2];
};