#ifndef GI_INTEGRATOR_PATH_TRACING_HPP
#define GI_INTEGRATOR_PATH_TRACING_HPP

#include "core/scene.hpp"
#include "core/camera.hpp"
#include "core/integrator.hpp"
#include "core/pdf.hpp"

namespace gi
{
    inline Float linear_to_gamma(Float linear_compontent)
    {
        if (linear_compontent > 0)
            return std::sqrt(linear_compontent);

        return 0;
    }

    class Path : public Integrator
    {
    public:
        // output image data
        Color *pOutput;

        SPtr<Camera> cam;
        SPtr<Scene> scn;
        void Render(const Hittable &world, const Hittable &lights) override
        {
            std::clog << "Path tracing rendering..." << std::endl;

            Int w = cam->GetWidth();
            Int h = cam->GetHeight();

            pOutput = new Color[w * h];
            // Parallel algorithm
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

                        Color pxColor(0.0f, 0.0f, 0.0f);
                        // for (Int sample = 0; sample < cam->spp; sample++)
                        // {
                        //     Ray r = cam->GetRay(i, j);
                        //     pxColor += Li(r, cam->maxDepth); // FIXME: scene argument 추가
                        // }

                        for (Int sj = 0; sj < cam->sqrtSpp; sj++)
                        {
                            for (Int si = 0; si < cam->sqrtSpp; si++)
                            {
                                Ray r = cam->GetRay(i, j, si, sj);
                                pxColor += Li(r, cam->maxDepth, world, lights);
                            }
                        }

                        pOutput[j * w + i] = (pxColor * cam->sppInv);
                    }
                }
            }
            FILE *f = fopen("output.ppm", "w"); // Write image to PPM file.
            fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
            for (Int i = 0; i < w * h; i++)
            {
                auto r = pOutput[i].x();
                auto g = pOutput[i].y();
                auto b = pOutput[i].z();

                if (r != r)
                    r = 0.0;
                if (g != g)
                    g = 0.0;
                if (b != b)
                    b = 0.0;

                r = linear_to_gamma(r);
                g = linear_to_gamma(g);
                b = linear_to_gamma(b);

                static const Interval intensity(0.000, 0.999);
                Int rbyte = Int(256 * intensity.Clamp(r));
                Int gbyte = Int(256 * intensity.Clamp(g));
                Int bbyte = Int(256 * intensity.Clamp(b));
                fprintf(f, "%d %d %d ", rbyte, gbyte, bbyte);
            }
            fclose(f);

            delete[] pOutput;
            std::clog << "\rDone.            ";
        }

        Color Li(const Ray &r, Int depth, const Hittable &world, const Hittable &lights) const
        {
            if (depth <= 0)
                return Color(0, 0, 0);

            HitRecord rec;
            if (!world.Hit(r, Interval(0.001, INF), rec))
                return scn->background;

            ScatterRecord srec;
            Color colorFromEmission = rec.mat->Emit(r, rec, rec.u, rec.v, rec.P);

            if (!rec.mat->Scatter(r, rec, srec))
                return colorFromEmission;

            if (srec.isSkipPdf)
            {
                return srec.attenuation * Li(srec.skipPdfRay, depth - 1, world, lights);
            }

            auto lightPtr = MakeShared<HittablePdf>(lights, rec.P);
            MixPdf p(lightPtr, srec.pdfPtr);

            Ray scattered = Ray(rec.P, p.Gen(), r.Time());
            auto pdf = p.Value(scattered.Dir());

            Float scatteringPdf = rec.mat->ScatteringPdf(r, rec, scattered);

            Color sampleColor = Li(scattered, depth - 1, world, lights);
            Color colorFromScatter = (srec.attenuation * scatteringPdf * sampleColor) / pdf;

            return colorFromEmission + colorFromScatter;
        }
    };
}

#endif