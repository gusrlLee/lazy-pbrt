#include "integrator/path.h"
#include "path.h"

inline F32 linear_to_gamma(F32 linear_compontent)
{
    if (linear_compontent > 0)
        return std::sqrt(linear_compontent);

    return 0;
}

void PathTracer::Render(const HitTable &world, const HitTable &lights)
{
    std::clog << "Path tracing rendering..." << std::endl;

    I32 w = cam->GetWidth();
    I32 h = cam->GetHeight();

    pOutput = new Color[w * h];
    // BF - algorithm
    // for (I32 j = 0; j < h; j++)
    // {
    //     for (I32 i = 0; i < w; i++)
    //     {
    //         std::clog << "\rScanlines remaining : " << F64(j) / F64(h) * 100.0 << " %      " << std::flush;

    //         Color pxColor(0.0f, 0.0f, 0.0f);
    //         for (I32 sample = 0; sample < cam->spp; sample++)
    //         {
    //             Ray r = cam->GetRay(i,j);
    //             pxColor += Li(r, cam->maxDepth);
    //         }
    //         pOutput[j * w + i] = (pxColor * cam->sppInv);
    //     }
    // }

// Parallel algorithm
#pragma omp parallel for schedule(dynamic)
    for (I32 tile = 0; tile < ((w * h) / (kTileSizeX * kTileSizeY)); tile++)
    {
        I32 x = tile % (w / kTileSizeX);
        I32 y = tile / (w / kTileSizeY);

        for (I32 v = 0; v < kTileSizeY; v++)
        {
            for (I32 u = 0; u < kTileSizeX; u++)
            {
                I32 i = kTileSizeX * x + u;
                I32 j = kTileSizeY * y + v;

                Color pxColor(0.0f, 0.0f, 0.0f);
                // for (I32 sample = 0; sample < cam->spp; sample++)
                // {
                //     Ray r = cam->GetRay(i, j);
                //     pxColor += Li(r, cam->maxDepth); // FIXME: scene argument 추가
                // }

                for (I32 sj = 0; sj < cam->sqrtSpp; sj++)
                {
                    for (I32 si = 0; si < cam->sqrtSpp; si++)
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
    for (I32 i = 0; i < w * h; i++)
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
        I32 rbyte = I32(256 * intensity.Clamp(r));
        I32 gbyte = I32(256 * intensity.Clamp(g));
        I32 bbyte = I32(256 * intensity.Clamp(b));
        fprintf(f, "%d %d %d ", rbyte, gbyte, bbyte);
    }
    fclose(f);

    delete[] pOutput;
    std::clog << "\rDone.                                                 \n";
}

// TODO:
// - Ray tracing pipeline class 를 생성해서 수정하기
// - Recusive function format 이 아닌 iteration format으로 변경하기
// - world and lights 는 Scene class 로 묶기
Color PathTracer::Li(const Ray &r, I32 depth, const HitTable &world, const HitTable &lights) const
{
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord rec;
    if (!world.Hit(r, Interval(0.001, inf), rec))
        return scn->background;

    ScatterRecord srec;
    Color colorFromEmission = rec.mat->Emit(r, rec, rec.u, rec.v, rec.P);

    if (!rec.mat->Scatter(r, rec, srec))
        return colorFromEmission;

    if (srec.isSkipPdf)
    {
        return srec.attenuation * Li(srec.skipPdfRay, depth - 1, world, lights);
    }

    auto lightPtr = MakeSptr<HitTablePdf>(lights, rec.P);
    MixPdf p(lightPtr, srec.pdfPtr);

    Ray scattered = Ray(rec.P, p.Gen(), r.Time());
    auto pdf = p.Value(scattered.Dir());

    F32 scatteringPdf = rec.mat->ScatteringPdf(r, rec, scattered);

    Color sampleColor = Li(scattered, depth - 1, world, lights);
    Color colorFromScatter = (srec.attenuation * scatteringPdf * sampleColor) / pdf;

    return colorFromEmission + colorFromScatter;
}