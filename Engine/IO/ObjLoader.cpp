#include "ObjLoader.h"

#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"

#include <iostream>
#include <filesystem>

#include "Engine/EngineManager.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "Library/tinyobjloader/tiny_obj_loader.h"

namespace XenonEngine
{
	using Algorithm::String;
    using Algorithm::Vector;
	using CrossPlatform::Polygon3D;
	using CrossPlatform::Material;
	using MathLab::Vector3f;
	using MathLab::Vector2f;

	//bool ObjectLoader::LoadObj(const Algorithm::String& fileName, Vector<Polygon3D*>& polygons, Vector<Polygon3D*>& materials)const
	//{

	//}

	bool ObjectLoader::LoadObj(const Algorithm::String& filePath, Algorithm::Vector<CrossPlatform::Polygon3D*>& polygons, Algorithm::Vector<CrossPlatform::Material*>& materials) const
	{
		std::string inputfile(filePath.Beign(), filePath.Count());
		tinyobj::ObjReaderConfig reader_config;
		//reader_config.mtl_search_path = "./"; // Path to material files

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(inputfile, reader_config)) {
			if (!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
			assert(true == false);
			return false;
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& objMaterials = reader.GetMaterials();

		int numOfVertex = (int)attrib.vertices.size() / 3;
		Vector3f* vertices = new Vector3f[numOfVertex];
		for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		{
			vertices[i / 3].x = attrib.vertices[i + 0];
			vertices[i / 3].y = attrib.vertices[i + 1];
			vertices[i / 3].z = attrib.vertices[i + 2];
		}

		int numOfNormal = (int)attrib.normals.size() / 3;
		Vector3f* normals = nullptr;
		if (numOfNormal > 0)
		{
			normals = new Vector3f[numOfNormal];
			for (size_t i = 0; i < attrib.normals.size(); i += 3)
			{
				normals[i / 3].x = attrib.normals[i + 0];
				normals[i / 3].y = attrib.normals[i + 1];
				normals[i / 3].z = attrib.normals[i + 2];
			}
		}
		int numOfTextureCoordinate = (int)attrib.texcoords.size() / 2;
		Vector2f* uv = nullptr;
		if (numOfTextureCoordinate > 0)
		{
			uv = new Vector2f[numOfTextureCoordinate];
			for (size_t i = 0; i < attrib.texcoords.size(); i+= 2)
			{
				uv[i / 2].x = attrib.texcoords[i + 0];
				uv[i / 2].y = attrib.texcoords[i + 1];
			}
		}

		for (const auto & objMaterial : objMaterials)
		{
			Material* material = new Material;
			material->m_name = objMaterial.name.c_str();
			material->m_ambient = Vector3f(objMaterial.ambient[0], objMaterial.ambient[1], objMaterial.ambient[2]);
			material->m_diffuse = Vector3f(objMaterial.diffuse[0], objMaterial.diffuse[1], objMaterial.diffuse[2]);
			material->m_specular = Vector3f(objMaterial.specular[0], objMaterial.specular[1], objMaterial.specular[2]);
			material->m_emission = Vector3f(objMaterial.emission[0], objMaterial.emission[1], objMaterial.emission[2]);
			String m_diffuseTextureFileName = objMaterial.diffuse_texname.c_str();
			String m_bumpTextureFileName = objMaterial.bump_texname.c_str();

			int pos = filePath.LastIndexOf(std::filesystem::path::preferred_separator);
			String modelFolder(filePath.Substring(0, pos + 1));
			if (!m_diffuseTextureFileName.Empty())
			{
				String diffuseTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_diffuseTextureFileName, modelFolder);
				IFileMeta* m_diffuseTexture = EngineManager::Get().GetFileDatabase().LoadFile(diffuseTextureFileName);
				assert(m_diffuseTexture != nullptr);
				material->m_diffuseTexture = m_diffuseTexture->GetFileHeader().GetGUID();
			}
			if (!m_bumpTextureFileName.Empty())
			{
				String bumpTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_bumpTextureFileName, modelFolder);
				IFileMeta* m_bumpTexture = EngineManager::Get().GetFileDatabase().LoadFile(bumpTextureFileName);
				assert(m_bumpTexture != nullptr);
				material->m_bumpTexture = m_bumpTexture->GetFileHeader().GetGUID();
			}

			//material->loadTextureData(filePath);
			materials.Add(material);
		}

		size_t vindex = 0;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++)
		{
			int numOfIndex = (int) shapes[s].mesh.indices.size();
			Polygon3D::VertexIndexs* vertexIndexList = new Polygon3D::VertexIndexs[numOfIndex];
			int numOfMaterial = (int) shapes[s].mesh.material_ids.size();
			int* materialIndex = nullptr;
			if (numOfMaterial > 0)
			{
				materialIndex = new int[numOfMaterial];
			}
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertexIndexList[vindex].m_vertexIndex = idx.vertex_index;
					if (idx.normal_index >= 0)
					{
						vertexIndexList[vindex].m_normalIndex = idx.normal_index;
					}
					if (idx.texcoord_index >= 0)
					{
						vertexIndexList[vindex].m_textureCoordinateIndex = idx.texcoord_index;
					}
					vindex++;
				}
				index_offset += fv;
				materialIndex[f] = shapes[s].mesh.material_ids[f];
			}
			Polygon3D* polygon = new Polygon3D(numOfIndex, vertexIndexList, numOfVertex, vertices, numOfNormal, normals, numOfTextureCoordinate, uv, numOfMaterial, materialIndex);
			polygons.Add(polygon);
		}

		return true;
	}

}