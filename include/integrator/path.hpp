#ifndef GI_INTEGRATOR_PATH_TRACING_HPP
#define GI_INTEGRATOR_PATH_TRACING_HPP

#include "core/integrator.hpp"

namespace gi
{
    class Path : public Integrator
    {
    public:
        Int spp = 16;
        Int maxDepth = 50;

        SPtr<Camera> pCam;
        SPtr<Scene> pScn;

        Path(SPtr<Camera> cam, SPtr<Scene> scn) : pCam(cam), pScn(scn) {
            result = new Color[pCam->GetWidth() * pCam->GetHeight()];
        }

        ~Path() override
        {
            delete[] result;
        }

        Color Li(const Ray& ray, RayPayload &payload) {

            // TODO: we have to implementation this part! 
            Ray r = ray;
            Color output = { 0.0f, 0.0f, 0.0f };
            Vec3f radiance = { 0.0f, 0.0f, 0.0f };
            Vec3f tp = {1.0f, 1.0f, 1.0f};

            // FIXME: we have to use Material class!
            Vec3f bxdf = {0.8f, 0.8f, 0.8f};

            for (Int i = 0; i < maxDepth; i++)
            {
                // tracing by using embree4 library
                if (!pScn->TraceRay(r, payload)) {
                    break;
                }

                // hit calculation 
                Triangle &hitTri = pScn->objects[payload.geomIdx]->tris[payload.primIdx];

                // get normal vector of hit trianlge
                payload.n = hitTri.n;

                // normal vector checking 
                Bool isFrontFace = Dot(r.d, payload.n) < 0;
                payload.n = isFrontFace ? payload.n : Reverse(payload.n);

                // light 
                if (hitTri.matIdx == 4) 
                {
                    Vec3f Le = Mul(tp, Vec3f(20.0f, 20.0f, 20.0f));
                    radiance = Add(radiance, Le);
                }

                // Lambertian diffuse 
                Float pdf = Dot(r.d, payload.n);
                tp = Mul(tp, bxdf); // bxdf 
                tp = Div(Mul(tp, pdf), pdf); // cosine term

                // for next depth
                // Lambertian
                Vec3f newDir = Sample::Direction();
                newDir = Add(payload.n, newDir);
                newDir = Normalize(newDir);

                // ray At function  
                Point3f newP = Add(r.o, Mul(r.d, payload.t));
                r = {newP, newDir};
            }

            return ConvertToColor(radiance);
        }

        void Render() override
        {
            Int w = pCam->GetWidth();
            Int h = pCam->GetHeight();

#pragma omp parallel for schedule(dynamic)
            for (Int tile = 0; tile < ((w * h) / (kTileSizeX * kTileSizeY)); tile++)
            {
                Int x = tile % (w / kTileSizeX);
                Int y = tile / (w / kTileSizeY);

                for (Int v = 0; v < kTileSizeY; v++)
                {
                    for (Int u = 0; u < kTileSizeX; u++)
                    {
                        Int i = kTileSizeX * x + u;
                        Int j = kTileSizeY * y + v;
                        for (int s = 0; s < spp; s++)
                        {
                            Vec2f offset = Sample::InPixel();
                            // generate ray for tracing
                            Point2i px = {i, j};

                            Ray ray = pCam->GenRay(px, offset);
                            RayPayload payload;

                            result[j * w + i] = Add(result[j * w + i], this->Li(ray, payload));
                        }
                        result[j * w + i] = Div(result[j * w + i], Float(spp));
                    }
                }
            }

            FILE *f = fopen("output.ppm", "w"); // Write image to PPM file.
            fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
            for (Int i = 0; i < w * h; i++)
            {
                UColor uColor = ToInt(result[i]);
                fprintf(f, "%d %d %d ", uColor.r, uColor.g, uColor.b);
            }
            fclose(f);
        }
        private:
            Color* result;
    };
} // namespace gi

#endif
