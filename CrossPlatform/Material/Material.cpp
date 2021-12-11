#include "Material.h"
#include "Engine/EngineManager.h"
#include "CrossPlatform/File/ImageMeta.h"

namespace CrossPlatform
{
	using namespace XenonEngine;
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

	void Material::loadTextureData()
	{
		if (!m_diffuseTextureFileName.Empty())
		{
			m_diffuseTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().LoadFile(m_diffuseTextureFileName);
		}
		if (!m_bumpTextureFileName.Empty())
		{
			m_bumpTexture = (ImageMeta*)EngineManager::Get().GetFileDatabase().LoadFile(m_bumpTextureFileName);
		}
	}

	const CrossPlatform::Image* Material::GetDiffuseTexture()
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

	const CrossPlatform::Image* Material::GetBumpTexture()
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