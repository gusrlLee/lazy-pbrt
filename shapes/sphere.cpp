#include "shapes/sphere.h"
#include "sphere.h"


// Stationary Sphere 
Sphere::Sphere(const Point3 &static_center, F32 radius, std::shared_ptr<Material> mat) : center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) 
{
    auto rvec = Vec3(radius, radius, radius);
    bbox = AABB(static_center - rvec, static_center + rvec);
}
// Moving Sphere
Sphere::Sphere(const Point3 &center1, const Point3 &center2, F32 radius, std::shared_ptr<Material> mat) : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) 
{
    auto rvec = Vec3(radius, radius, radius);
    AABB box1(center.At(0) - rvec, center.At(0) + rvec);
    AABB box2(center.At(1) - rvec, center.At(1) + rvec);
    bbox = AABB(box1, box2);
}

bool Sphere::Hit(const Ray &r, Interval t, HitRecord &rec) const 
{
    Point3 currCenter = center.At(r.Time());
    Vec3 oc = currCenter - r.Orig();
    auto a = r.Dir().LengthSqaured();
    auto h = Dot(r.Dir(), oc);
    auto c = oc.LengthSqaured() - radius * radius;

    auto d = h * h - a * c;
    if (d < 0)
        return false;

    auto sqrtd = sqrt(d);
    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!t.Surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!t.Surrounds(root))
            return false;
    }

    rec.t = root;
    rec.P = r.At(rec.t);
    Vec3 outNormal = (rec.P - currCenter) / radius;
    rec.SetFaceNormal(r, outNormal);
    GetSphereUV(outNormal, rec.u, rec.v);
    rec.mat = mat;

    return true;
}

F32 Sphere::PdfValue(const Point3 &origin, const Vec3 &dir) const
{
    HitRecord rec;
    if (!this->Hit(Ray(origin, dir), Interval(0.001, inf), rec))
        return 0;
    
    auto distSq = (center.At(0) - origin).LengthSqaured();
    auto cosThetaMax = std::sqrt(1 - radius * radius / distSq);
    auto solidAngle = 2 * pi * (1 - cosThetaMax);

    return 1 / solidAngle;
}

void Sphere::GetSphereUV(const Point3 &p, F32 &u, F32 &v)
{
    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;
    u = phi / (2 * pi);
    v = theta / pi;
}
