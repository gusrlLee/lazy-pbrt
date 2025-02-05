#ifndef GI_CORE_MESH_HPP
#define GI_CORE_MESH_HPP

#include "core/geometry.hpp"
#include "core/materials.hpp"
#include "tiny_obj_loader.h"


namespace gi
{
    class Mesh
    {
        public:
            std::vector<Vertex> vertices;
            std::vector<UInt> indices;
            std::vector<UInt> matIndices;
            std::vector<Triangle> tris;

            Mesh(const String &path, const String &fName, const String &type)
            {
                // obj file and material file path 
                String strFilePath = path + '/' + fName + type;
                String strMatPath = path + '/';

                fPath = strFilePath.c_str();
                mPath = strMatPath.c_str();

                Init();
            }

        private:
            void Init()
            {
                // tinyobjloader configuration
                tinyobj::ObjReaderConfig cfg;
                cfg.mtl_search_path = mPath;
                cfg.triangulate = true; // required 

                // read obj file from path
                tinyobj::ObjReader reader;
                if (!reader.ParseFromFile(fPath, cfg))
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
                for (Size s = 0; s < shapes.size(); s++)
                {
                    // Loop over faces (polygon)
                    Size idxOffset = 0;
                    for (Size f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
                    {
                        Size fv = Size(shapes[s].mesh.num_face_vertices[f]);
                        std::vector<Vertex> vbuf; // vertices of triangle

                        for (Size v = 0; v < fv; v++) // fv size = 9
                        { // access to vertex
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

                        // store material index of triangle in vector
                        matIndices.push_back(shapes[s].mesh.material_ids[f]);
                        vbuf.clear();
                        idxOffset += fv;
                    }
                } // for
                
                for (Int i = 0; i < (indices.size() / 3); i++)
                {
                    Triangle t;

                    t.v0 = vertices[3 * indices[i] + 0];
                    t.v1 = vertices[3 * indices[i] + 1];
                    t.v2 = vertices[3 * indices[i] + 2];

                    t.e1 = Sub(t.v1, t.v0);
                    t.e2 = Sub(t.v2, t.v0);
                    t.n = Normalize(Cross(t.e1, t.e2));

                    t.matIdx = matIndices[i];
                    tris.push_back(t);
                }
#if 1
                std::clog << "vertices size = " << vertices.size() << std::endl;
                std::clog << "indices size = " << indices.size() << std::endl;
                std::clog << "indices size / 3 = " << indices.size() / 3 << std::endl;
                std::clog << "material size = " << matIndices.size() << std::endl;
                std::clog << "triangle size = " << tris.size() << std::endl;
#endif
            }

            CString *fPath;
            CString *mPath;
    };
}

#endif
