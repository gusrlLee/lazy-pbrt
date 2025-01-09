#include "core/types.h"
#include "math/vec3.h"
#include "core/color.h"

#include "core/random.h"

#include <iostream>

#include "core/camera.h"
#include "core/scene.h"
#include "core/hittablelist.h"
#include "core/material.h"

#include "material/lambertian.h"
#include "material/metal.h"
#include "material/dielectric.h"

#include "integrator/path.h"

#include "shapes/sphere.h"
#include "shapes/quad.h"

#include "lights/diffuse.h"

#include "accel/bvh.h"

HitTableList Test1()
{
    HitTableList world;
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Dielectric>(1.50);
    auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    return world;
}

HitTableList Test2()
{
    HitTableList world;
    auto checker = MakeSptr<CheckerTexture>(0.32, Color(.2, .3, .1), Color(0.9, 0.9, 0.9));
    // auto grountMat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, MakeSptr<Lambertian>(checker)));

    for (I32 a = -11; a < 11; a++)
    {
        for (I32 b = -11; b < 11; b++)
        {
            auto choose_mat = Random::Value();
            Point3 center(a + 0.9 * Random::Value(), 0.2, b + 0.9 * Random::Value());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8)
                {
                    auto albedo = Random::GenColor() * Random::GenColor();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, Random::Value(0, 0.5), 0);
                    world.Add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = Random::GenColor(0.5, 1);
                    auto fuzz = Random::Value(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

HitTableList Test3()
{
    // camera setting 
    // cam.aspect_ratio = 16.0 / 9.0;
    // cam.image_width = 400;
    // cam.samples_per_pixel = 100;
    // cam.max_depth = 50;

    // cam.vfov = 20;
    // cam.lookfrom = point3(13, 2, 3);
    // cam.lookat = point3(0, 0, 0);
    // cam.vup = Vec3(0, 1, 0);

    // cam.defocus_angle = 0;

    HitTableList world;
    auto checker = MakeSptr<CheckerTexture>(0.32, Color(.2, .3, .1), Color(0.9, 0.9, 0.9));
    world.Add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, MakeSptr<Lambertian>(checker)));
    world.Add(std::make_shared<Sphere>(Point3(0,  10, 0), 10, MakeSptr<Lambertian>(checker)));

    return world;
}

HitTableList Test4()
{
    HitTableList world;
    auto earthTex = MakeSptr<ImageTexture>("../assets/images/earthmap.jpg");
    auto earthSurf = MakeSptr<Lambertian>(earthTex);

    world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, earthSurf));
    return world;
}

HitTableList Test5()
{
    HitTableList world;
    auto perTex = MakeSptr<NoiseTexture>(4);
    world.Add(MakeSptr<Sphere>(Point3(0,-1000,0), 1000, MakeSptr<Lambertian>(perTex))); 
    world.Add(MakeSptr<Sphere>(Point3(0, 2, 0), 2, MakeSptr<Lambertian>(perTex))); 

    return world;
}

HitTableList Test6()
{
    HitTableList world;

    // Materials
    auto left_red     = MakeSptr<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green   = MakeSptr<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue   = MakeSptr<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = MakeSptr<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal   = MakeSptr<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.Add(MakeSptr<Quad>(Point3(-3,-2, 5), Vec3(0, 0,-4), Vec3(0, 4, 0), left_red));
    world.Add(MakeSptr<Quad>(Point3(-2,-2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.Add(MakeSptr<Quad>(Point3( 3,-2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.Add(MakeSptr<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.Add(MakeSptr<Quad>(Point3(-2,-3, 5), Vec3(4, 0, 0), Vec3(0, 0,-4), lower_teal));
    return world;
}

HitTableList SimpleLight()
{
    HitTableList world;
    auto perTex = MakeSptr<NoiseTexture>(4);
    world.Add(MakeSptr<Sphere>(Point3(0,-1000,0), 1000, MakeSptr<Lambertian>(perTex))); 
    world.Add(MakeSptr<Sphere>(Point3(0, 2, 0), 2, MakeSptr<Lambertian>(perTex))); 

    auto diffuseLight = MakeSptr<DiffuseLight>(Color(4, 4, 4));
    world.Add(MakeSptr<Sphere>(Point3(0, 7, 0), 2, diffuseLight)); 
    world.Add(MakeSptr<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), diffuseLight));

    return world;
}

HitTableList CornellBox()
{
    HitTableList world;

    auto red   = MakeSptr<Lambertian>(Color(.65, .05, .05));
    auto white = MakeSptr<Lambertian>(Color(.73, .73, .73));
    auto green = MakeSptr<Lambertian>(Color(.12, .45, .15));
    auto light = MakeSptr<DiffuseLight>(Color(15, 15, 15));

    world.Add(MakeSptr<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.Add(MakeSptr<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.Add(MakeSptr<Quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light));
    world.Add(MakeSptr<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.Add(MakeSptr<Quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white));
    world.Add(MakeSptr<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));
    
    return world;
}

int main()
{
    // setting camera
    std::shared_ptr<Camera> pCam = std::make_shared<Camera>();

    pCam->aspectRatio = 1.0f;
    pCam->imgWidth = 600;
    pCam->spp = 200;
    pCam->maxDepth = 50;

    pCam->vFov = 40;
    pCam->lookfrom = Point3(278, 278, -800);
    pCam->lookat = Point3(278, 278, 0);
    pCam->vup = Vec3(0,1,0);

    pCam->defocusAngle = 0.0f;
    pCam->Init();

    // setting scene of rendering
    auto world = CornellBox();

    world = HitTableList(MakeSptr<BVHNode>(world));
    std::shared_ptr<Scene> pScene = std::make_shared<Scene>();
    pScene->world = std::make_shared<HitTableList>(world);
    pScene->background = Color(0, 0, 0);

    // setting path tracer
    std::shared_ptr<PathTracer> pIntegrator = std::make_shared<PathTracer>();
    pIntegrator->cam = pCam;
    pIntegrator->scn = pScene;

    pIntegrator->Render();

    return 0;
}