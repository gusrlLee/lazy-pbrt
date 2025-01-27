#ifndef GI_CORE_SCENE_HPP
#define GI_CORE_SCENE_HPP

#include "core/geometry.hpp"
#include "core/hittablelist.hpp"
#include "accel/bvh.hpp"

#include "shapes/quad.hpp"
#include "shapes/sphere.hpp"
#include "shapes/triangle.hpp"

#include "tiny_obj_loader.h"

namespace gi
{
    class Scene
    {
    public:
        Scene() {}
        ~Scene() {}

        std::vector<Vertex> vertices;
        std::vector<Hittable> primitives;

        HittableList world;
        HittableList lights;

        Color background;

        void SetCornellBox()
        {
            auto red = MakeShared<Lambertian>(Color(.65, .05, .05));
            auto white = MakeShared<Lambertian>(Color(.73, .73, .73));
            auto green = MakeShared<Lambertian>(Color(.12, .45, .15));
            auto light = MakeShared<DiffuseLight>(Color(15, 15, 15));
            auto aluminum = MakeShared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
            auto glass = MakeShared<Dielectric>(1.5);

            world.Add(MakeShared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
            world.Add(MakeShared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
            world.Add(MakeShared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
            world.Add(MakeShared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
            world.Add(MakeShared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
            world.Add(MakeShared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

            SPtr<Hittable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
            box1 = MakeShared<RotateY>(box1, 15);
            box1 = MakeShared<Translate>(box1, Vec3(265, 0, 295));
            world.Add(box1);

            // Sptr<HitTable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
            // box2 = MakeShared<RotateY>(box2, -18);
            // box2 = MakeShared<Translate>(box2, Vec3(130, 0, 65));
            // world.Add(box2);

            world.Add(MakeShared<Sphere>(Point3(190, 90, 190), 90, glass));

            // setting background color
            background = Color(0.0f);

            // setting bvh tree
            world = HittableList(MakeShared<BVHNode>(world));
        }

        void LoadObj(CString *path)
        {
            auto white = MakeShared<Lambertian>(Color(.73, .73, .73));
            auto light = MakeShared<DiffuseLight>(Color(15, 15, 15));

            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path = "../assets/models"; // Path to material files

            tinyobj::ObjReader reader;

            if (!reader.ParseFromFile(path, reader_config))
            {
                if (!reader.Error().empty())
                {
                    std::cerr << "TinyObjReader: " << reader.Error();
                }
                exit(1);
            }

            if (!reader.Warning().empty())
            {
                std::cout << "TinyObjReader: " << reader.Warning();
            }

            auto &attrib = reader.GetAttrib();
            auto &shapes = reader.GetShapes();
            auto &materials = reader.GetMaterials();

            // Loop over shapes
            for (size_t s = 0; s < shapes.size(); s++)
            {
                // Loop over faces(polygon)
                size_t index_offset = 0;
                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
                {
                    size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                    // Loop over vertices in the face.
                    for (size_t v = 0; v < fv; v++)
                    {
                        // access to vertex
                        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                        // Check if `normal_index` is zero or positive. negative = no normal data
                        if (idx.normal_index >= 0)
                        {
                            tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                            tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                            tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                        }

                        // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                        if (idx.texcoord_index >= 0)
                        {
                            tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                            tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                        }


                        vertices.push_back(Vertex(vx, vy, vz));

                        // Optional: vertex colors
                        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                    }

                    index_offset += fv;

                    // per-face material
                    shapes[s].mesh.material_ids[f];
                }
            } // for 
            std::cout << "vertices size - " << vertices.size() << std::endl;
            for (Int idx = 0; idx < vertices.size() / 3; idx++)
            {
                world.Add(MakeShared<Triangle>(vertices[3 * idx + 0], vertices[3 * idx + 1], vertices[3 * idx + 2], white));
            }
            std::cout << "World triangle size = " << world.Size() << std::endl;
            world.Add(MakeShared<Quad>(Point3(0, 10, 0), Vec3(-10, 0, 0), Vec3(0, 0, -10), light));

            // setting background color
            background = Color(0.0f);

            // setting bvh tree
            world = HittableList(MakeShared<BVHNode>(world));
        }
    };

}

#endif