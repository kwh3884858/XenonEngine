#include "Material.h"
#include "Engine/EngineManager.h"
#include "CrossPlatform/File/ImageMeta.h"
#include <filesystem>
namespace CrossPlatform
{
	using namespace XenonEngine;
	using namespace Algorithm;
 //   Material::Material(const Algorithm::String& name, float exponent, const MathLab::Vector3f& ambient, const MathLab::Vector3f& diffuse, const MathLab::Vector3f& specular, const MathLab::Vector3f& emission, const Algorithm::String& diffuseTexture) :
 //       m_name(name),
 //       m_exponent(exponent),
 //       m_ambient(ambient),
 //       m_diffuse(diffuse),
 //       m_specular(specular),
 //       m_emission(emission),
 //       m_diffuseTexture(diffuseTexture)
 //   {
 //   }

	//Material::Material(const Material& material) :
	//	m_name(material.m_name),
	//	m_exponent(material.m_exponent),
	//	m_ambient(material.m_ambient),
	//	m_diffuse(material.m_diffuse),
	//	m_specular(material.m_specular),
	//	m_emission(material.m_emission),
	//	m_diffuseTextureFileName(material.m_diffuseTextureFileName),
	//	m_diffuseTexture(material.m_diffuseTexture),
	//	m_bumpTextureFileName(material.m_bumpTextureFileName),
	//	m_bumpTexture(material.m_bumpTexture)
	//{
	//}

	//Material::~Material()
	//{
	//}

	//void Material::loadTextureData(const Algorithm::String& modelPath)
	//{
	//	int pos = modelPath.LastIndexOf(std::filesystem::path::preferred_separator);
	//	String modelFolder(modelPath.Substring(0, pos + 1));
	//	if (!m_diffuseTextureFileName.Empty())
	//	{
	//		String diffuseTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_diffuseTextureFileName, modelFolder);
	//		m_diffuseTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().LoadFile(diffuseTextureFileName);
	//		if (!m_diffuseTexture)
	//		{
	//			m_diffuseTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().AddFile(diffuseTextureFileName);
	//			assert(m_diffuseTexture != nullptr);
	//		}
	//	}
	//	if (!m_bumpTextureFileName.Empty())
	//	{
	//		String bumpTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_bumpTextureFileName, modelFolder);
	//		m_bumpTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().LoadFile(bumpTextureFileName);
	//		if (!m_bumpTexture)
	//		{
	//			m_bumpTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().AddFile(bumpTextureFileName);
	//			assert(m_bumpTexture != nullptr);
	//		}
	//	}
	//}

	const Algorithm::String& Material::GetDiffuseTextureFileName() const
	{
		return EngineManager::Get().GetFileDatabase().GetFile(m_diffuseTexture)->GetFileHeader().GetFileName();
	}

	const Algorithm::String& Material::GetBumpTextureName() const
	{
		return EngineManager::Get().GetFileDatabase().GetFile(m_bumpTexture)->GetFileHeader().GetFileName();
	}

	CrossPlatform::Image* Material::GetDiffuseTexture()
	{
		if (m_diffuseTexture.isValid())
		{
			ImageMeta* imageMeta = (ImageMeta*)EngineManager::Get().GetFileDatabase().GetFile(m_diffuseTexture);
			return imageMeta->GetImage();
		}
		else
		{
			return nullptr;
		}
	}

	CrossPlatform::Image* Material::GetBumpTexture()
	{
		if (m_bumpTexture.isValid())
		{
			ImageMeta* imageMeta = (ImageMeta*)EngineManager::Get().GetFileDatabase().GetFile(m_bumpTexture);
			return imageMeta->GetImage();
		}
		else
		{
			return nullptr;
		}
	}

}