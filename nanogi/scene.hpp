#ifndef NANO_GLOBAL_ILLUMINATION_SCENE_HPP
#define NANO_GLOBAL_ILLUMINATION_SCENE_HPP

#include "nanogi/data.hpp"
#include "nanogi/material.hpp"

#include "embree4/rtcore.h"
#include "tiny_obj_loader.h"

namespace nanogi
{
    class Mesh
    {
    public:
        Int nTris;
        std::vector<Vertex> vertices;
        std::vector<Int> indices;
        std::vector<Int> matIndices;
        std::vector<Triangle> tris;
        std::vector<Triangle> lights; // TODO: 이 부분의 light 부분을 scene 에서 접근 가능하도록 만들어야 함.

        Mesh(const std::string &dPath, const std::string &fName, const std::string &type)
        {
            // obj file and material file path
            std::string objPath = dPath + '/' + fName + type;
            std::string mtlPath = dPath + '/';

            Init(objPath.c_str(), mtlPath.c_str());
        }

    private:
        void Init(const char *obj, const char *mtl)
        {
            // tinyobjloader configuration
            tinyobj::ObjReaderConfig cfg;
            cfg.mtl_search_path = mtl;
            cfg.triangulate = true; // required

            // read obj file from path
            tinyobj::ObjReader reader;
            if (!reader.ParseFromFile(obj, cfg))
            {
                if (!reader.Error().empty())
                {
                    std::cerr << "[MESH ERROR] : " << reader.Error();
                    exit(1);
                }
            }
            if (!reader.Warning().empty())
            {
                std::clog << "[MESH WARNING] : " << reader.Warning() << std::endl;
            }
            // Parsing obj file
            const auto &attrib = reader.GetAttrib();
            const auto &shapes = reader.GetShapes();
            const auto &materials = reader.GetMaterials();

            // Loop over shapes
            for (size_t s = 0; s < shapes.size(); s++)
            {
                // Loop over faces (polygon)
                size_t idxOffset = 0;
                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
                {
                    size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
                    std::vector<Vertex> vbuf; // vertices of triangle

                    for (size_t v = 0; v < fv; v++) // fv size = 9
                    {                               // access to vertex
                        tinyobj::index_t idx = shapes[s].mesh.indices[idxOffset + v];

                        // get vertex of triangle
                        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                        // create vertex data  (x, y, z)
                        vbuf.push_back(Vertex(vx, vy, vz));
                    }

                    for (Int i = 0; i < vbuf.size(); i++)
                    {
                        // merge vertex data into total vector
                        vertices.push_back(vbuf[i]);
                        // sync vertex indiex & index of vertex for triangle
                        // vertex 1 = index 1
                        indices.push_back(indices.size());
                    }

                    matIndices.push_back(shapes[s].mesh.material_ids[f]);
                    vbuf.clear();
                    idxOffset += fv;
                }
            } // for

            for (Int idx = 0; idx < (indices.size() / 3); idx++)
            {
                Triangle tri;
                Int matIdx = matIndices[idx];

                Color diffuse = Color(
                    materials[matIdx].diffuse[0],
                    materials[matIdx].diffuse[1],
                    materials[matIdx].diffuse[2]);

                Color light = Color(
                    materials[matIdx].ambient[0],
                    materials[matIdx].ambient[1],
                    materials[matIdx].ambient[2]);

                if (matIndices[idx] == 4) // light
                {
                    tri = Triangle(
                        vertices[3 * indices[idx] + 0],            // v0
                        vertices[3 * indices[idx] + 1],            // v1
                        vertices[3 * indices[idx] + 2],            // v2
                        std::make_shared<DiffuseLight>(light)); // material

                    // light triangle save 
                    lights.push_back(tri);
                }
                else
                {
                    tri = Triangle(
                        vertices[3 * indices[idx] + 0],   // v0
                        vertices[3 * indices[idx] + 1],   // v1
                        vertices[3 * indices[idx] + 2],   // v2
                        std::make_shared<Lambertian>(diffuse)); // material
                }
                tris.push_back(tri);
            }

#if 0
            std::clog << "vertices size = " << vertices.size() << std::endl;
            std::clog << "indices size = " << indices.size() << std::endl;
            std::clog << "indices size / 3 = " << indices.size() / 3 << std::endl;
            std::clog << "material size = " << materials.size() << std::endl;
            std::clog << "material index size = " << matIndices.size() << std::endl;
            std::clog << "triangle size = " << tris.size() << std::endl;
            std::clog << "light triangle size = " << lights.size() << std::endl;
#endif
        }
    };

    class Scene
    {
    public:
        std::vector<std::shared_ptr<Mesh>> objects;

        RTCDevice dev;
        RTCScene scn;

        Scene()
        {
            dev = rtcNewDevice(NULL);
            if (!dev)
                printf("error %d: cannot create device\n", rtcGetDeviceError(NULL));

            scn = rtcNewScene(dev);
        }

        ~Scene()
        {
            rtcReleaseScene(scn);
            rtcReleaseDevice(dev);
        }

        void AddObject(std::shared_ptr<Mesh> mesh) { objects.push_back(mesh); }
        void AddObject(const std::string &dPath, const std::string &fName, const std::string &type) {
            objects.push_back(std::make_shared<Mesh>(dPath, fName, type));
        }

        Int Size() { return objects.size(); }
        void Clear() { objects.clear(); }

        void Build()
        {
            for (Int i = 0; i < objects.size(); i++)
            {
                Add(objects[i]);
            }
            rtcCommitScene(scn);
        }

        bool TraceRay(const Ray &ray, Payload &payload)
        {
            struct RTCRayHit rayhit;
            rayhit.ray.org_x = ray.Orig().x();
            rayhit.ray.org_y = ray.Orig().y();
            rayhit.ray.org_z = ray.Orig().z();
            rayhit.ray.dir_x = ray.Dir().x();
            rayhit.ray.dir_y = ray.Dir().y();
            rayhit.ray.dir_z = ray.Dir().z();
            rayhit.ray.tnear = 0;
            rayhit.ray.tfar = std::numeric_limits<float>::infinity();
            rayhit.ray.mask = -1;
            rayhit.ray.flags = 0;
            rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
            rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

            rtcIntersect1(scn, &rayhit);

            if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
            {
#if 0
                    printf("Found intersection on geometry %d, primitive %d at tfar=%f\n",
                            rayhit.hit.geomID,
                            rayhit.hit.primID,
                            rayhit.ray.tfar);

#endif
                payload.hRec.t = rayhit.ray.tfar;
                payload.hRec.gID = rayhit.hit.geomID;
                payload.hRec.pID = rayhit.hit.primID;
                payload.hRec.p = ray.At(rayhit.ray.tfar);

                Triangle &hitTri = objects[payload.hRec.gID]->tris[payload.hRec.pID];

                Normal n = hitTri.Normal();
                payload.hRec.n = Dot(ray.Dir(), n) < 0 ? n : -n; // setting front face 
                payload.hRec.mat = hitTri.mat;
                
                return true;
            }

            return false;
        }

    private:
        void Add(std::shared_ptr<Mesh> mesh)
        {
            RTCGeometry geom = rtcNewGeometry(dev, RTC_GEOMETRY_TYPE_TRIANGLE);
            float *vertices = (float *)rtcSetNewGeometryBuffer(geom,
                                                               RTC_BUFFER_TYPE_VERTEX,
                                                               0,
                                                               RTC_FORMAT_FLOAT3,
                                                               3 * sizeof(float),
                                                               mesh->vertices.size());

            unsigned *indices = (unsigned *)rtcSetNewGeometryBuffer(geom,
                                                                    RTC_BUFFER_TYPE_INDEX,
                                                                    0,
                                                                    RTC_FORMAT_UINT3,
                                                                    3 * sizeof(unsigned),
                                                                    mesh->indices.size());

            for (Int idx = 0; idx < mesh->indices.size(); idx++)
            {
                vertices[3 * idx + 0] = mesh->vertices[idx].x();
                vertices[3 * idx + 1] = mesh->vertices[idx].y();
                vertices[3 * idx + 2] = mesh->vertices[idx].z();
                indices[idx] = mesh->indices[idx];
            }

            rtcCommitGeometry(geom);
            rtcAttachGeometry(this->scn, geom);
            rtcReleaseGeometry(geom);
        }
    };
}

#endif
