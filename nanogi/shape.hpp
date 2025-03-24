#ifndef NANO_GLOBAL_ILLUMINATION_SHAPE_HPP
#define NANO_GLOBAL_ILLUMINATION_SHAPE_HPP

#include "nanogi/data.hpp"
#include "nanogi/random.hpp"

namespace nanogi
{
    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual bool Hit(const Ray &r, Payload &payload) const = 0;
        virtual Float PdfValue(const Ray &ray) const
        {
            return 0.0;
        }

        virtual Vec3 Sample(const Point3 &origin) const
        {
            return Vec3(1, 0, 0);
        }
    };

    class Triangle : public Shape
    {
    public:
        // vertices
        Vertex v[3];
        std::shared_ptr<Material> mat;

        Triangle() : v{Vec3(0), Vec3(0), Vec3(0)} {}
        Triangle(Vertex v0, Vertex v1, Vertex v2) : v{v0, v1, v2} {}
        Triangle(Vertex v0, Vertex v1, Vertex v2, std::shared_ptr<Material> mat) : v{v0, v1, v2}, mat(mat) {}

        Vec3 Edge1() const
        {
            return v[1] - v[0];
        }

        Vec3 Edge2() const
        {
            return v[2] - v[0];
        }

        Vec3 Normal() const
        {
            return Normalize(Cross(Edge1(), Edge2()));
        }

        Float Area() const
        {
            return Cross(Edge1(), Edge2()).Len() / 2;
        }

        bool Hit(const Ray &ray, Payload &payload) const override
        {
            Vec3 e1 = Edge1();
            Vec3 e2 = Edge2();

            // Möller-Trumbore 알고리즘
            Vec3 h = Cross(ray.Dir(), e2);
            float a = Dot(e1, h);

            if (fabs(a) < Eps)
                return false; // 평행한 경우 교차 없음

            float f = 1.0f / a;
            Vec3 s = ray.Orig() - v[0];
            float u = f * Dot(s, h);

            if (u < 0.0f || u > 1.0f)
                return false;

            Vec3 q = Cross(s, e1);
            float v = f * Dot(ray.Dir(), q);

            if (v < 0.0f || (u + v) > 1.0f)
                return false;

            // 교차 거리 t 계산
            Float t = f * Dot(e2, q);

            if (t < Eps)
                return false; // 교차점이 음수면 뒤쪽에서 발생한 충돌 (무시)

            // 교차점과 법선 계산
            payload.hRec.t = t;
            payload.hRec.p = ray.Orig() + t * ray.Dir();
            Vec3 n = Normal();
            payload.hRec.n = Dot(ray.Dir(), n) < 0 ? n : -n; // setting front face

            return true; // 교차 발생
        }

        Float PdfValue(const Ray &ray) const override
        {
            Payload payload;
            if (!this->Hit(ray, payload))
                return 0;

            auto distSq = payload.hRec.t * payload.hRec.t * ray.Dir().LenSq();
            auto cosine = std::fabs(Dot(ray.Dir(), payload.hRec.n) / ray.Dir().Len());
            return distSq / (cosine * this->Area());
        }

        Vec3 Sample(const Point3 &origin) const override
        {
            Float r1 = Random::Get1D();
            Float r2 = Random::Get1D();

            // Barycentric coordinates 변환
            Float sqrtR1 = std::sqrt(r1);
            Float u = 1 - sqrtR1;
            Float v = r2 * sqrtR1;

            auto p = this->v[0] * u + this->v[1] * v + this->v[2] * (1 - u - v);
            return Normalize(p - origin);
        }
    };
} // namespace nanogi

#endif
