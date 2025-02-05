#ifndef GI_CORE_SCENE_HPP
#define GI_CORE_SCENE_HPP

#include "core/geometry.hpp"
#include "core/mesh.hpp"

#include <embree4/rtcore.h>
#include <iostream>

namespace gi
{
    class Scene
    {
        public:
            std::vector<SPtr<Mesh>> objects;

            RTCDevice dev;
            RTCScene scn;

            Scene()
            {
                // create embree device 
                dev = rtcNewDevice(NULL);
                if (!dev)
                {
                    printf("error %d: cannot create device\n", rtcGetDeviceError(NULL));
                }
                scn = rtcNewScene(dev);
            }

            ~Scene()
            {
                rtcReleaseScene(scn);
                rtcReleaseDevice(dev);
            }

            void AddObject(SPtr<Mesh> mesh) { objects.push_back(mesh); }
            UInt Size() { return objects.size(); }
            void Clear() { objects.clear(); }

            void Build()
            {
                for (Int i = 0; i < objects.size(); i++)
                {
                    Add(objects[i]);
                }
                rtcCommitScene(scn);
            }

            Bool TraceRay(const Ray &ray, RayPayload &payload)
            {
                struct RTCRayHit rayhit;
                rayhit.ray.org_x = ray.o.x;
                rayhit.ray.org_y = ray.o.y;
                rayhit.ray.org_z = ray.o.z;
                rayhit.ray.dir_x = ray.d.x;
                rayhit.ray.dir_y = ray.d.y;
                rayhit.ray.dir_z = ray.d.z;
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
                    payload.t = rayhit.ray.tfar;
                    payload.geomIdx = rayhit.hit.geomID;
                    payload.primIdx = rayhit.hit.primID;
                    return true;
                }

                return false;
            }

        private:
            void Add(SPtr<Mesh> mesh)
            {
                RTCGeometry geom = rtcNewGeometry(dev, RTC_GEOMETRY_TYPE_TRIANGLE);
                Vertex *vertices = (Vertex *)rtcSetNewGeometryBuffer(geom,
                        RTC_BUFFER_TYPE_VERTEX,
                        0,
                        RTC_FORMAT_FLOAT3,
                        sizeof(Vertex),
                        mesh->vertices.size());

                UInt *indices = (UInt *)rtcSetNewGeometryBuffer(geom,
                        RTC_BUFFER_TYPE_INDEX,
                        0,
                        RTC_FORMAT_UINT3,
                        3 * sizeof(UInt),
                        mesh->indices.size());

                for (Int idx = 0; idx < mesh->indices.size(); idx++)
                {
                    vertices[idx] = mesh->vertices[idx];
                    indices[idx] = mesh->indices[idx];
                }

                rtcCommitGeometry(geom);
                rtcAttachGeometry(this->scn, geom);
                rtcReleaseGeometry(geom);
            }
    };

} // namespace gi

#endif
