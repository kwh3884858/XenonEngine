#include "ObjLoader.h"

#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"

#include <iostream>
#include <filesystem>

#include "CrossPlatform/File/Mesh3DMeta.h"

#include "Engine/EngineManager.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "Library/tinyobjloader/tiny_obj_loader.h"
#include "Engine/Component/Mesh3D.h"
#include "CrossPlatform/File/MaterialMeta.h"
#include "CrossPlatform/File/Polygon3DMeta.h"

namespace XenonEngine
{
	using namespace Algorithm;
	using namespace CrossPlatform;
	using namespace MathLab;
	//using namespace std;

	bool ObjectLoader::LoadObj(const Algorithm::String& path) const
	{
		// Path constant
		int delimiterIndex = path.LastIndexOf(std::filesystem::path::preferred_separator);
		assert(delimiterIndex >= 0);
		const String fileName = path.Substring(delimiterIndex + 1, path.Count());

		int pos = path.LastIndexOf(std::filesystem::path::preferred_separator);
		const String modelFolder(path.Substring(0, pos + 1));

		std::string inputfile(path.CString());
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
		Vector<Vector3f> vertexs;
		vertexs.Initialize(numOfVertex);
		for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		{
			vertexs[i / 3].x = attrib.vertices[i + 0];
			vertexs[i / 3].y = attrib.vertices[i + 1];
			vertexs[i / 3].z = attrib.vertices[i + 2];
		}

		int numOfNormal = (int)attrib.normals.size() / 3;
		Vector<Vector3f> normals;
		normals.Initialize(numOfNormal);
		if (numOfNormal > 0)
		{
			//normals = new Vector3f[numOfNormal];
			for (size_t i = 0; i < attrib.normals.size(); i += 3)
			{
				normals[i / 3].x = attrib.normals[i + 0];
				normals[i / 3].y = attrib.normals[i + 1];
				normals[i / 3].z = attrib.normals[i + 2];
			}
		}
		int numOfTextureCoordinate = (int)attrib.texcoords.size() / 2;

		Vector<Vector2f> uv;
		if (numOfTextureCoordinate > 0)
		{
			uv.Initialize(numOfTextureCoordinate);
			for (size_t i = 0; i < attrib.texcoords.size(); i+= 2)
			{
				uv[i / 2].x = attrib.texcoords[i + 0];
				uv[i / 2].y = attrib.texcoords[i + 1];
			}
		}

		//Save to mesh3D
		std::string meshName = std::filesystem::path::preferred_separator +
			fileName.CString() +
			EngineManager::Get().GetFileDatabase().GetExtension(FileType::FileTypeMaterial).CString();
		String meshPath = modelFolder + meshName.c_str();
		Mesh3DMeta* meshMeta = (Mesh3DMeta*)EngineManager::Get().GetFileDatabase().CreateMetaFromFilePath(meshPath);
		
		Mesh3D* mesh = new Mesh3D();
		mesh->m_vertexs = std::move(vertexs);
		mesh->m_normals = std::move(normals);
		mesh->m_uv = std::move(uv);

		for (const auto & objMaterial : objMaterials)
		{
			Material* material = new Material;
			material->m_name = objMaterial.name.c_str();
			material->m_ambient = Vector3f(objMaterial.ambient[0], objMaterial.ambient[1], objMaterial.ambient[2]);
			material->m_diffuse = Vector3f(objMaterial.diffuse[0], objMaterial.diffuse[1], objMaterial.diffuse[2]);
			material->m_specular = Vector3f(objMaterial.specular[0], objMaterial.specular[1], objMaterial.specular[2]);
			material->m_emission = Vector3f(objMaterial.emission[0], objMaterial.emission[1], objMaterial.emission[2]);
			material->m_diffuseTexture = xg::Guid();
			material->m_bumpTexture = xg::Guid();


			String m_diffuseTextureFileName = objMaterial.diffuse_texname.c_str();
			if (!m_diffuseTextureFileName.Empty())
			{
				String diffuseTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_diffuseTextureFileName, modelFolder);
				IFileMeta* m_diffuseTexture = EngineManager::Get().GetFileDatabase().CreateMetaFromFilePath(diffuseTextureFileName);
				assert(m_diffuseTexture != nullptr);
				material->m_diffuseTexture = m_diffuseTexture->GetFileHeader().GetGUID();
			}

			String m_bumpTextureFileName = objMaterial.bump_texname.c_str();
			if (!m_bumpTextureFileName.Empty())
			{
				String bumpTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_bumpTextureFileName, modelFolder);
				IFileMeta* m_bumpTexture = EngineManager::Get().GetFileDatabase().CreateMetaFromFilePath(bumpTextureFileName);
				assert(m_bumpTexture != nullptr);
				material->m_bumpTexture = m_bumpTexture->GetFileHeader().GetGUID();
			}
			std::string materialFileName = std::filesystem::path::preferred_separator + 
				objMaterial.name.c_str() + 
				EngineManager::Get().GetFileDatabase().GetExtension(FileType::FileTypeMaterial).CString();
			String materialPath = modelFolder + materialFileName.c_str();
			MaterialMeta* materialMeta =(MaterialMeta*) EngineManager::Get().GetFileDatabase().CreateMetaFromFilePath(materialPath);
			materialMeta->m_material = material;
			materialMeta->Save();

			mesh->m_materials.Add(materialMeta->GetFileHeader().GetGUID());
		}

		size_t vindex = 0;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++)
		{

			//int numOfMaterial = (int) shapes[s].mesh.material_ids.size();
			//int* materialIndex = nullptr;
			//if (numOfMaterial > 0)
			//{
			//	materialIndex = new int[numOfMaterial];
			//}
			// Loop over faces(polygon)
			Vector<Polygon3D::VertexIndex> vertexIndex;
			int numOfIndex = (int)shapes[s].mesh.indices.size();
			vertexIndex.Initialize(numOfIndex);
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertexIndex[vindex].m_vertexIndex = idx.vertex_index;
					if (idx.normal_index >= 0)
					{
						vertexIndex[vindex].m_normalIndex = idx.normal_index;
					}
					if (idx.texcoord_index >= 0)
					{
						vertexIndex[vindex].m_textureCoordinateIndex = idx.texcoord_index;
					}
					vindex++;
				}
				index_offset += fv;
				vertexIndex[vindex].m_materialIndex = shapes[s].mesh.material_ids[f];
			}
			//Create new polygon3D
			std::string polygonName = std::filesystem::path::preferred_separator +
				fileName.CString() + s +
				EngineManager::Get().GetFileDatabase().GetExtension(FileType::FileTypePolygon).CString();
			String polygonPath = modelFolder + polygonName.c_str();
			Polygon3DMeta* polygonMeta = (Polygon3DMeta*)EngineManager::Get().GetFileDatabase().CreateMetaFromFilePath(polygonPath);

			Mesh3D* mesh = new Mesh3D();
			Polygon3D* polygon = new Polygon3D(std::move(vertexIndex));

			polygonMeta->m_polygon = polygon;
			polygonMeta->Save();

			mesh->m_polygons.Add(polygonMeta->GetFileHeader().GetGUID());
		}

		return true;
	}

}