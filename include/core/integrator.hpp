#ifndef GI_CORE_INTEGRATOR_HPP
#define GI_CORE_INTEGRATOR_HPP

#include "core/geometry.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"

namespace gi
{
    class Integrator
    {
    public:
        Int spp = 10;
        Int maxDepth = 5;

        SPtr<Camera> pCam;
        SPtr<Scene> pScn;

        Integrator(SPtr<Camera> cam, SPtr<Scene> scn) : pCam(cam), pScn(scn)
        {
            result = new Color[pCam->GetWidth() * pCam->GetHeight()];
        }
        ~Integrator()
        {
            delete[] result;
        }

        void Render()
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

                        // generate ray for tracing  
                        Point2i px = {i, j};
                        Ray ray = pCam->GenRay(px);

                        Vec3f unitDir = Normalize(ray.d);
                        Float a = 0.5 * (unitDir.y + 1.0);
                        Vec3f left = Mul(Vec3f{1.0f, 1.0f, 1.0f}, (1.0f - a));
                        Vec3f right = Mul(Vec3f{0.5f, 0.7, 1.0f}, a);
                        Vec3f resColor = Add(left, right);

                        result[j * w + i] = Color{resColor.x, resColor.y, resColor.z};
                    }
                }
            }

            FILE *f = fopen("output.ppm", "w"); // Write image to PPM file.
            fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
            for (Int i = 0; i < w * h; i++)
            {
                auto r = result[i].r;
                auto g = result[i].g;
                auto b = result[i].b;

                int ir = int(255.999 * r);
                int ig = int(255.999 * g);
                int ib = int(255.999 * b);
                fprintf(f, "%d %d %d ", ir, ig, ib);
            }
            fclose(f);
        }

    private:
        const Int kTileSizeX = 8;
        const Int kTileSizeY = 8;
        Color *result;
    };
} // namespace gi

#endif // GI_CORE_INTEGRATOR_HPP