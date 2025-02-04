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
                                                            3 * sizeof(unsigned),
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