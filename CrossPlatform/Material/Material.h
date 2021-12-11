#pragma once

#include "Algorithms/String.h"
#include "MathLab/Vector3.h"

namespace CrossPlatform
{
	class ImageMeta;
	class Image;
    class Material
    {
    public:
		Material() = default;
		~Material() = default;

		const Algorithm::String& GetName()const { return m_name; }
		const float& GetExponent()const { return m_exponent; }
		const MathLab::Vector3f& GetAmbient()const { return m_ambient; }
		const MathLab::Vector3f& GetDiffuse()const { return m_diffuse; }
		const MathLab::Vector3f& GetSpecular()const { return m_specular; }
		const MathLab::Vector3f& GetEmission()const { return m_emission; }
		const Algorithm::String& GetDiffuseTextureFileName()const { return m_diffuseTextureFileName; }
		const Algorithm::String& GetBumpTextureName()const { return m_bumpTextureFileName; }

		void loadTextureData();
		const Image* GetDiffuseTexture();
		const Image* GetBumpTexture();
	public:
		Algorithm::String m_name;
        float m_exponent = 0.0f;                        //Ns
        MathLab::Vector3f m_ambient;                    //Ka
        MathLab::Vector3f m_diffuse;                    //Kd
        MathLab::Vector3f m_specular;                   //Ks
        MathLab::Vector3f m_emission;                   //Ke
		Algorithm::String m_diffuseTextureFileName;              // map_Kd
		ImageMeta* m_diffuseTexture;
		Algorithm::String m_bumpTextureFileName;					// map_bump
		ImageMeta* m_bumpTexture;
    };

}