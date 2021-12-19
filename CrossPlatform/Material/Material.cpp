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

	//Material::~Material()
	//{
	//}

	void Material::loadTextureData(const Algorithm::String& modelPath)
	{
		int pos = modelPath.LastIndexOf(std::filesystem::path::preferred_separator);
		String modelFolder(modelPath.Substring(0, pos + 1));
		if (!m_diffuseTextureFileName.Empty())
		{
			String diffuseTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_diffuseTextureFileName, modelFolder);
			m_diffuseTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().LoadFile(diffuseTextureFileName);
			if (!m_diffuseTexture)
			{
				m_diffuseTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().AddFile(diffuseTextureFileName);
				assert(m_diffuseTexture != nullptr);
			}
		}
		if (!m_bumpTextureFileName.Empty())
		{
			String bumpTextureFileName = EngineManager::Get().GetFileDatabase().ProcessFileName(m_bumpTextureFileName, modelFolder);
			m_bumpTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().LoadFile(bumpTextureFileName);
			if (!m_bumpTexture)
			{
				m_bumpTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().AddFile(bumpTextureFileName);
				assert(m_diffuseTexture != nullptr);
			}
		}
	}

	CrossPlatform::Image* Material::GetDiffuseTexture()
	{
		if (m_diffuseTexture)
		{
			return m_diffuseTexture->GetImage();
		}
		else
		{
			return nullptr;
		}
	}

	CrossPlatform::Image* Material::GetBumpTexture()
	{
		if (m_bumpTexture)
		{
			return m_bumpTexture->GetImage();
		}
		else
		{
			return nullptr;
		}
	}

}