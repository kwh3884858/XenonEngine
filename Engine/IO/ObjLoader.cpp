#include "ObjLoader.h"
#include "CrossPlatform/Polygon3D.h"
#include "MathLab/Vector3.h"
#include <iostream>


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
			exit(1);
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		int numOfVertex = attrib.vertices.size() / 3;
		Vector3f* verteces = new Vector3f[attrib.vertices.size() / 3];
		int numOfIndex = 0;
		int* vertexIndexList = new int[attrib.vertices.size() / 3];

		for (size_t i = 0; i < attrib.vertices.size(); i+=3)
		{
			verteces[i / 3].x = attrib.vertices[i + 0];
			verteces[i / 3].y = attrib.vertices[i + 1];
			verteces[i / 3].z = attrib.vertices[i + 2];
		}

		size_t vindex = 0;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
				for (size_t v = 0; v < fv; v++) 
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertexIndexList[vindex++] = idx.vertex_index;
				}
				index_offset += fv;
			}
		}

		Polygon3D* polygon = new Polygon3D(numOfVertex, vertexIndexList, numOfVertex, verteces);
		return polygon;
	}
}