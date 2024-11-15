#include "ObjectImporter.h"

#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"

#include <iostream>
#include <filesystem>

#include "CrossPlatform/File/Mesh3DMeta.h"
#include "CrossPlatform/File/MaterialMeta.h"
#include "CrossPlatform/File/Polygon3DMeta.h"

#include "CrossPlatform/Polygon/Polygon3D.h"
#include "CrossPlatform/Material/Material.h"

#include "Engine/EngineManager.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "Library/tinyobjloader/tiny_obj_loader.h"
#include "Engine/Component/Mesh3D.h"


namespace XenonEngine
{
	using namespace Algorithm;
	using namespace CrossPlatform;
	using namespace MathLab;
	//using namespace std;

	bool ObjectImporter::ImportObj(const Algorithm::String& path) const
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
		vertexs.ResetCount(numOfVertex);
		for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		{
			vertexs[(int)i / 3].x = attrib.vertices[(int)(i + 0)];
			vertexs[(int)i / 3].y = attrib.vertices[(int)(i + 1)];
			vertexs[(int)i / 3].z = attrib.vertices[(int)(i + 2)];
		}

		int numOfNormal = (int)attrib.normals.size() / 3;
		Vector<Vector3f> normals;
		normals.Initialize(numOfNormal);
		normals.ResetCount(numOfNormal);
		if (numOfNormal > 0)
		{
			//normals = new Vector3f[numOfNormal];
			for (size_t i = 0; i < attrib.normals.size(); i += 3)
			{
				normals[(int)i / 3].x = attrib.normals[(int)(i + 0)];
				normals[(int)i / 3].y = attrib.normals[(int)(i + 1)];
				normals[(int)i / 3].z = attrib.normals[(int)(i + 2)];
			}
		}
		int numOfTextureCoordinate = (int)attrib.texcoords.size() / 2;

		Vector<Vector2f> uv;
		if (numOfTextureCoordinate > 0)
		{
			uv.Initialize(numOfTextureCoordinate);
			uv.ResetCount(numOfTextureCoordinate);
			for (size_t i = 0; i < attrib.texcoords.size(); i+= 2)
			{
				uv[(int)i / 2].x = attrib.texcoords[(int)(i + 0)];
				uv[(int)i / 2].y = attrib.texcoords[(int)(i + 1)];
			}
		}

		//Save to mesh3D
		Mesh3D* mesh = new Mesh3D();
		//meshMeta->m_mesh = mesh;

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
				String diffuseTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileNameToFullPath(m_diffuseTextureFileName, modelFolder);
				IFileMeta* m_diffuseTexture = EngineManager::Get().GetFileDatabase().GenerateMetaFileForFile(diffuseTextureFileName);
				assert(m_diffuseTexture != nullptr);
				material->m_diffuseTexture = m_diffuseTexture->GetFileHeader().GetGUID();
			}

			String m_bumpTextureFileName = objMaterial.bump_texname.c_str();
			if (!m_bumpTextureFileName.Empty())
			{
				String bumpTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileNameToFullPath(m_bumpTextureFileName, modelFolder);
				IFileMeta* m_bumpTexture = EngineManager::Get().GetFileDatabase().GenerateMetaFileForFile(bumpTextureFileName);
				assert(m_bumpTexture != nullptr);
				material->m_bumpTexture = m_bumpTexture->GetFileHeader().GetGUID();
			}
			String materialPath = modelFolder + std::filesystem::path::preferred_separator +
				objMaterial.name.c_str() + 
				EngineManager::Get().GetFileDatabase().GetExtension(FileType::FileTypeMaterial);
			//String materialPath = modelFolder + materialFileName.c_str();
			MaterialMeta* materialMeta =(MaterialMeta*) EngineManager::Get().GetFileDatabase().GenerateMetaFileForFile(materialPath);
			//materialMeta->m_material = material;
			materialMeta->Save(material);
			mesh->m_materials.Add(materialMeta->GetFileHeader().GetGUID());

			delete material;
			material = nullptr;
		}

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++)
		{
			int vindex = 0;
			//int numOfMaterial = (int) shapes[s].mesh.material_ids.size();
			//int* materialIndex = nullptr;
			//if (numOfMaterial > 0)
			//{
			//	materialIndex = new int[numOfMaterial];
			//}
			// Loop over faces(polygon)
			Vector<Polygon3D::TriangleIndex> vertexIndex;
			int numOfIndex = (int)shapes[s].mesh.indices.size() / 3;
			vertexIndex.Initialize(numOfIndex);
			vertexIndex.ResetCount(numOfIndex);
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertexIndex[vindex].m_vertex[v].m_vertexIndex = idx.vertex_index;
					if (idx.normal_index >= 0)
					{
						vertexIndex[vindex].m_vertex[v].m_normalIndex = idx.normal_index;
					}
					if (idx.texcoord_index >= 0)
					{
						vertexIndex[vindex].m_vertex[v].m_textureCoordinateIndex = idx.texcoord_index;
					}
				}
				vertexIndex[vindex].m_materialIndex = shapes[s].mesh.material_ids[f];
				index_offset += fv;
				vindex++;
			}
			//Create new polygon3D
			String polygonPath = modelFolder + std::filesystem::path::preferred_separator +
				fileName + std::to_string(s).c_str() +
				EngineManager::Get().GetFileDatabase().GetExtension(FileType::FileTypePolygon);
			//String polygonPath = modelFolder + polygonName.c_str();
			Polygon3DMeta* polygonMeta = (Polygon3DMeta*)EngineManager::Get().GetFileDatabase().GenerateMetaFileForFile(polygonPath);

			//Mesh3D* mesh = new Mesh3D();
			Polygon3D* polygon = new Polygon3D(std::move(vertexIndex));

			//polygonMeta->m_polygon = polygon;
			polygonMeta->Save(polygon);
			mesh->m_polygons.Add(polygonMeta->GetFileHeader().GetGUID());

			delete polygon;
			polygon = nullptr;
		}
		
		String meshPath = 
			modelFolder +
			fileName +
			EngineManager::Get().GetFileDatabase().GetExtension(FileType::FileTypeMesh3D);
		Mesh3DMeta* meshMeta = (Mesh3DMeta*)EngineManager::Get().GetFileDatabase().GenerateMetaFileForFile(meshPath);
		meshMeta->Save(mesh);
		delete mesh;
		mesh = nullptr;

		return true;
	}

}