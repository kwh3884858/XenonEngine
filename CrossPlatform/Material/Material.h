#pragma once

#include "Algorithms/String.h"
#include "MathLab/Vector3.h"
#include "CrossPlatform/XenonShaderType.h"
#include "crossguid/guid.hpp"

namespace CrossPlatform
{
	class ImageMeta;
	class Image;
    class Material
    {
    public:
		Material() = default;
		~Material() = default;
		Material(const Material& material) = delete;
		Material(Material&& material);

		ShaderType GetShaderType()const { return m_shaderType; }

		const Algorithm::String& GetName()const { return m_name; }
		const float& GetExponent()const { return m_exponent; }
		const MathLab::Vector3f& GetAmbient()const { return m_ambient; }
		const MathLab::Vector3f& GetDiffuse()const { return m_diffuse; }
		const MathLab::Vector3f& GetSpecular()const { return m_specular; }
		const MathLab::Vector3f& GetEmission()const { return m_emission; }
		const Algorithm::String& GetDiffuseTextureFileName()const;
		const Algorithm::String& GetBumpTextureName()const;

		//void loadTextureData(const Algorithm::String& modelPath);
		Image* GetDiffuseTexture();
		Image* GetBumpTexture();
	public:
		ShaderType m_shaderType = ShaderType::ShaderType_Gouraud;

		Algorithm::String m_name;
        float m_exponent = 0.0f;                        //Ns
        MathLab::Vector3f m_ambient;                    //Ka
        MathLab::Vector3f m_diffuse;                    //Kd
        MathLab::Vector3f m_specular;                   //Ks
        MathLab::Vector3f m_emission;                   //Ke
		xg::Guid m_diffuseTexture;
		xg::Guid m_bumpTexture;

		//Algorithm::String m_diffuseTextureFileName;              // map_Kd
		//ImageMeta* m_diffuseTexture;
		//Algorithm::String m_bumpTextureFileName;					// map_bump
		//ImageMeta* m_bumpTexture;
    };

}