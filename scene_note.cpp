// HitTableList Test1()
// {
//     HitTableList world;
//     auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
//     auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
//     auto material_left = std::make_shared<Dielectric>(1.50);
//     auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
//     auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

//     world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
//     world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
//     world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
//     world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
//     world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

//     return world;
// }

// HitTableList Test2()
// {
//     HitTableList world;
//     auto checker = MakeSptr<CheckerTexture>(0.32, Color(.2, .3, .1), Color(0.9, 0.9, 0.9));
//     // auto grountMat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
//     world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, MakeSptr<Lambertian>(checker)));

//     for (I32 a = -11; a < 11; a++)
//     {
//         for (I32 b = -11; b < 11; b++)
//         {
//             auto choose_mat = Random::Value();
//             Point3 center(a + 0.9 * Random::Value(), 0.2, b + 0.9 * Random::Value());

//             if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
//             {
//                 std::shared_ptr<Material> sphere_material;
//                 if (choose_mat < 0.8)
//                 {
//                     auto albedo = Random::GenColor() * Random::GenColor();
//                     sphere_material = std::make_shared<Lambertian>(albedo);
//                     auto center2 = center + Vec3(0, Random::Value(0, 0.5), 0);
//                     world.Add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
//                 }
//                 else if (choose_mat < 0.95)
//                 {
//                     // metal
//                     auto albedo = Random::GenColor(0.5, 1);
//                     auto fuzz = Random::Value(0, 0.5);
//                     sphere_material = std::make_shared<Metal>(albedo, fuzz);
//                     world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
//                 }
//                 else
//                 {
//                     // glass
//                     sphere_material = std::make_shared<Dielectric>(1.5);
//                     world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
//                 }
//             }
//         }
//     }

//     auto material1 = std::make_shared<Dielectric>(1.5);
//     world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

//     auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
//     world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

//     auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
//     world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

//     return world;
// }

// HitTableList Test3()
// {
//     // camera setting
//     // cam.aspect_ratio = 16.0 / 9.0;
//     // cam.image_width = 400;
//     // cam.samples_per_pixel = 100;
//     // cam.max_depth = 50;

//     // cam.vfov = 20;
//     // cam.lookfrom = point3(13, 2, 3);
//     // cam.lookat = point3(0, 0, 0);
//     // cam.vup = Vec3(0, 1, 0);

//     // cam.defocus_angle = 0;

//     HitTableList world;
//     auto checker = MakeSptr<CheckerTexture>(0.32, Color(.2, .3, .1), Color(0.9, 0.9, 0.9));
//     world.Add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, MakeSptr<Lambertian>(checker)));
//     world.Add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, MakeSptr<Lambertian>(checker)));

//     return world;
// }

// HitTableList Test4()
// {
//     HitTableList world;
//     auto earthTex = MakeSptr<ImageTexture>("../assets/images/earthmap.jpg");
//     auto earthSurf = MakeSptr<Lambertian>(earthTex);

//     world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, earthSurf));
//     return world;
// }

// HitTableList Test5()
// {
//     HitTableList world;
//     auto perTex = MakeSptr<NoiseTexture>(4);
//     world.Add(MakeSptr<Sphere>(Point3(0, -1000, 0), 1000, MakeSptr<Lambertian>(perTex)));
//     world.Add(MakeSptr<Sphere>(Point3(0, 2, 0), 2, MakeSptr<Lambertian>(perTex)));

//     return world;
// }

// HitTableList Test6()
// {
//     HitTableList world;

//     // Materials
//     auto left_red = MakeSptr<Lambertian>(Color(1.0, 0.2, 0.2));
//     auto back_green = MakeSptr<Lambertian>(Color(0.2, 1.0, 0.2));
//     auto right_blue = MakeSptr<Lambertian>(Color(0.2, 0.2, 1.0));
//     auto upper_orange = MakeSptr<Lambertian>(Color(1.0, 0.5, 0.0));
//     auto lower_teal = MakeSptr<Lambertian>(Color(0.2, 0.8, 0.8));

//     // Quads
//     world.Add(MakeSptr<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), left_red));
//     world.Add(MakeSptr<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
//     world.Add(MakeSptr<Quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
//     world.Add(MakeSptr<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
//     world.Add(MakeSptr<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lower_teal));
//     return world;
// }

// HitTableList SimpleLight()
// {
//     HitTableList world;
//     auto perTex = MakeSptr<NoiseTexture>(4);
//     world.Add(MakeSptr<Sphere>(Point3(0, -1000, 0), 1000, MakeSptr<Lambertian>(perTex)));
//     world.Add(MakeSptr<Sphere>(Point3(0, 2, 0), 2, MakeSptr<Lambertian>(perTex)));

//     auto diffuseLight = MakeSptr<DiffuseLight>(Color(4, 4, 4));
//     world.Add(MakeSptr<Sphere>(Point3(0, 7, 0), 2, diffuseLight));
//     world.Add(MakeSptr<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), diffuseLight));

//     return world;
// }

// HitTableList CornellBox()
// {
//     HitTableList world;

//     auto red = MakeSptr<Lambertian>(Color(.65, .05, .05));
//     auto white = MakeSptr<Lambertian>(Color(.73, .73, .73));
//     auto green = MakeSptr<Lambertian>(Color(.12, .45, .15));
//     auto light = MakeSptr<DiffuseLight>(Color(15, 15, 15));
//     auto aluminum = MakeSptr<Metal>(Color(0.8, 0.85, 0.88), 0.0);
//     auto glass = MakeSptr<Dielectric>(1.5);

//     world.Add(MakeSptr<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
//     world.Add(MakeSptr<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
//     world.Add(MakeSptr<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
//     world.Add(MakeSptr<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
//     world.Add(MakeSptr<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
//     world.Add(MakeSptr<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

//     Sptr<HitTable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
//     box1 = MakeSptr<RotateY>(box1, 15);
//     box1 = MakeSptr<Translate>(box1, Vec3(265, 0, 295));
//     world.Add(box1);

//     // Sptr<HitTable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
//     // box2 = MakeSptr<RotateY>(box2, -18);
//     // box2 = MakeSptr<Translate>(box2, Vec3(130, 0, 65));
//     // world.Add(box2);

//     world.Add(MakeSptr<Sphere>(Point3(190, 90, 190), 90, glass));

//     return world;
// }

// HitTableList CornellSmoke()
// {
//     HitTableList world;

//     auto red = MakeSptr<Lambertian>(Color(.65, .05, .05));
//     auto white = MakeSptr<Lambertian>(Color(.73, .73, .73));
//     auto green = MakeSptr<Lambertian>(Color(.12, .45, .15));
//     auto light = MakeSptr<DiffuseLight>(Color(15, 15, 15));

//     world.Add(MakeSptr<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
//     world.Add(MakeSptr<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
//     world.Add(MakeSptr<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
//     world.Add(MakeSptr<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
//     world.Add(MakeSptr<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
//     world.Add(MakeSptr<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

//     Sptr<HitTable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
//     box1 = MakeSptr<RotateY>(box1, 15);
//     box1 = MakeSptr<Translate>(box1, Vec3(265, 0, 295));

//     Sptr<HitTable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
//     box2 = MakeSptr<RotateY>(box2, -18);
//     box2 = MakeSptr<Translate>(box2, Vec3(130, 0, 65));

//     world.Add(MakeSptr<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
//     world.Add(MakeSptr<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

//     return world;
// }

// HitTableList RTNWFinalWorldScene()
// {
//     HitTableList boxes1;
//     auto ground = MakeSptr<Lambertian>(Color(0.48, 0.83, 0.53));

//     int boxes_per_side = 20;
//     for (int i = 0; i < boxes_per_side; i++)
//     {
//         for (int j = 0; j < boxes_per_side; j++)
//         {
//             auto w = 100.0;
//             auto x0 = -1000.0 + i * w;
//             auto z0 = -1000.0 + j * w;
//             auto y0 = 0.0;
//             auto x1 = x0 + w;
//             auto y1 = Random::Value(1, 101);
//             auto z1 = z0 + w;

//             boxes1.Add(Box(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
//         }
//     }

//     HitTableList world;

//     world.Add(MakeSptr<BVHNode>(boxes1));

//     auto light = MakeSptr<DiffuseLight>(Color(7, 7, 7));
//     world.Add(MakeSptr<Quad>(Point3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light));

//     auto center1 = Point3(400, 400, 200);
//     auto center2 = center1 + Vec3(30, 0, 0);
//     auto sphere_material = MakeSptr<Lambertian>(Color(0.7, 0.3, 0.1));
//     world.Add(MakeSptr<Sphere>(center1, center2, 50, sphere_material));

//     world.Add(MakeSptr<Sphere>(Point3(260, 150, 45), 50, MakeSptr<Dielectric>(1.5)));
//     world.Add(MakeSptr<Sphere>(
//         Point3(0, 150, 145), 50, MakeSptr<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

//     auto boundary = MakeSptr<Sphere>(Point3(360, 150, 145), 70, MakeSptr<Dielectric>(1.5));
//     world.Add(boundary);
//     world.Add(MakeSptr<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
//     boundary = MakeSptr<Sphere>(Point3(0, 0, 0), 5000, MakeSptr<Dielectric>(1.5));
//     world.Add(MakeSptr<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

//     auto emat = MakeSptr<Lambertian>(MakeSptr<ImageTexture>("../assets/images/earthmap.jpg"));
//     world.Add(MakeSptr<Sphere>(Point3(400, 200, 400), 100, emat));
//     auto pertext = MakeSptr<NoiseTexture>(0.2);
//     world.Add(MakeSptr<Sphere>(Point3(220, 280, 300), 80, MakeSptr<Lambertian>(pertext)));

//     HitTableList boxes2;
//     auto white = MakeSptr<Lambertian>(Color(.73, .73, .73));
//     int ns = 1000;
//     for (int j = 0; j < ns; j++)
//     {
//         boxes2.Add(MakeSptr<Sphere>(Random::Vector3(0, 165), 10, white));
//     }

//     world.Add(MakeSptr<Translate>(
//         MakeSptr<RotateY>(
//             MakeSptr<BVHNode>(boxes2), 15),
//         Vec3(-100, 270, 395)));

//     return world;
// }
