#include "ObjLoader.h"
#include "CrossPlatform/Polygon3D.h"
#include "MathLab/Vector3.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "Library/tiny_obj_loader.h"
namespace XenonEngine
{
	using Algorithm::String;
	using CrossPlatform::Polygon3D;
	using MathLab::Vector3f;

	const Polygon3D* ObjectLoader::LoadObj(Algorithm::String & fileName)
	{
		std::string inputfile (fileName.Beign(), fileName.Count());
		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = "./"; // Path to material files

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(inputfile, reader_config)) {
			if (!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
			assert(true == false);
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		int numOfVertex = attrib.vertices.size() / 3;
		Vector3f* vertices = new Vector3f[numOfVertex];
		for (size_t i = 0; i < attrib.vertices.size(); i+=3)
		{
			vertices[i / 3].x = attrib.vertices[i + 0];
			vertices[i / 3].y = attrib.vertices[i + 1];
			vertices[i / 3].z = attrib.vertices[i + 2];
		}

        int numOfNormal = attrib.normals.size / 3;
        Vector3f* normals = new Vector3f[numOfNormal];
        for (size_t i = 0; i < attrib.normals.size(); i++)
        {
            normals[i / 3].x = attrib.normals[i + 0];
            normals[i / 3].y = attrib.normals[i + 1];
            normals[i / 3].z = attrib.normals[i + 2];
        }

        int numOfIndex = 0;
		size_t vindex = 0;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
            int numOfIndex = shapes[s].mesh.indices.size();
            Polygon3D::VertexIndexs* vertexIndexList = new Polygon3D::VertexIndexs[numOfIndex];
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
				for (size_t v = 0; v < fv; v++) 
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertexIndexList[vindex].m_vertexIndex = idx.vertex_index;
                    if (idx.normal_index >=0)
                    {
                        vertexIndexList[vindex].m_normalIndex = idx.normal_index;
                    }
                    if (idx.texcoord_index >=0)
                    {
                        vertexIndexList[vindex].m_textureCoordinateIndex = idx.texcoord_index;
                    }
                    vindex++;
				}
				index_offset += fv;
			}
            Polygon3D* polygon = new Polygon3D(numOfIndex, vertexIndexList, numOfVertex, vertices, numOfNormal, normals);
            return polygon;
		}

        return nullptr;
	}
}