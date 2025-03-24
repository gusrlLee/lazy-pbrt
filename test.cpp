#include "nanogi/nanogi.hpp"

using namespace nanogi;

const Int kTileSizeX = 8;
const Int kTileSizeY = 8;

inline Float LinearToGamma(Float component)
{
    if (component > 0)
        return std::sqrt(component);

    return 0;
}

int main()
{
    // setting scene
    auto pScn = std::make_shared<nanogi::Scene>();
    pScn->AddObject("../assets/models", "cornell_box", ".obj");
    pScn->Build();

    nanogi::Int w = 600;
    nanogi::Int h = 600;

    // setting camera to render the cornell_box
    auto pCam = std::make_shared<nanogi::Camera>();

    Int spp = 1024;
    Int sqrtSpp = Int(std::sqrt(spp));
    Float recipSqrtSpp = 1.0 / sqrtSpp;

    Int maxDepth = 50;
    Color *output = new Color[w * h];

// path tracing start
#pragma omp parallel for schedule(dynamic)
    for (Int tile = 0; tile < ((w * h) / (kTileSizeX * kTileSizeY)); tile++)
    {
        // setting tile size
        Int x = tile % (w / kTileSizeX);
        Int y = tile / (w / kTileSizeY);

        for (Int v = 0; v < kTileSizeY; v++)
        {
            for (Int u = 0; u < kTileSizeX; u++)
            {
                Int i = kTileSizeX * x + u;
                Int j = kTileSizeY * y + v;

                Color result;

                for (Int sj = 0; sj < sqrtSpp; sj++)
                {
                    for (Int si = 0; si < sqrtSpp; si++)
                    {
                        auto pixel = Point2(i, j);
                        auto offset = Point2(
                            ((si + Random::Get1D()) * recipSqrtSpp) - 0.5,
                            ((sj + Random::Get1D()) * recipSqrtSpp) - 0.5);

                        Ray ray = pCam->GenRay(pixel, offset);
                        Payload payload;

                        Vec3 Lo = Vec3(0.0); // final radiance value
                        Vec3 tp = Vec3(1.0); // throughput

                        Ray wi = ray; // just name transform

                        for (Int depth = 0; depth < maxDepth; depth++)
                        {
                            if (!pScn->TraceRay(wi, payload))
                            {
                                break; // ray miss
                            }

                            if (payload.hRec.mat->IsLight())
                            {
                                // if material is light, ray traversal step is terminated
                                Lo += payload.hRec.mat->Le(wi, payload) * tp;
                                break;
                            }

                            Vec3 scatter = payload.hRec.mat->F(wi, payload);                            

                            auto p0 = std::make_shared<LightPdf>(pScn->objects[payload.hRec.gID]->lights, payload.hRec.p);
                            auto p1 = std::make_shared<CosinePdf>(payload.hRec.n);

                            MixPdf mixPdf(p0, p1);
                            payload.sRec.nextRay = Ray(payload.hRec.p, mixPdf.Gen());
                            
                            // pdf 
                            Float pdf = mixPdf.Value(payload.sRec.nextRay.Dir());
                            
                            // cosine term
                            Float sPdf = payload.hRec.mat->Pdf(wi, payload);
                        
                            tp = (tp * scatter * sPdf) / pdf;
                            wi = payload.sRec.nextRay;
                        }
                        result += Lo;
                    }
                }
                output[j * w + i] = result / spp;
            }
        }
    }



    FILE *f = fopen("output.ppm", "w"); // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (Int i = 0; i < w * h; i++)
    {
        auto r = output[i].r();
        auto g = output[i].g();
        auto b = output[i].b();

        if(r != r) r = 0.0;
        if(g != g) g = 0.0;
        if(b != b) b = 0.0;

        r = LinearToGamma(r);
        g = LinearToGamma(g);
        b = LinearToGamma(b);

        Int rByte = Int(256 * std::clamp(r, (Float)0.0000, (Float)0.9999));
        Int gByte = Int(256 * std::clamp(g, (Float)0.0000, (Float)0.9999));
        Int bByte = Int(256 * std::clamp(b, (Float)0.0000, (Float)0.9999));

        fprintf(f, "%d %d %d ", rByte, gByte, bByte);
    }
    fclose(f);
    delete[] output;
    return 0;
}