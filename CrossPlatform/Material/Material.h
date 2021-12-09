#pragma once

#include "Algorithms/String.h"
#include "MathLab/Vector3.h"

namespace CrossPlatform
{
	using namespace MathLab;
	using namespace Algorithm;
    class Material
    {
    public:
		Material() = default;
		~Material() = default;

		const String& GetName()const { return m_name; }
		const float& GetExponent()const { return m_exponent; }
		const Vector3f& GetAmbient()const { return m_ambient; }
		const Vector3f& GetDiffuse()const { return m_diffuse; }
		const Vector3f& GetSpecular()const { return m_specular; }
		const Vector3f& GetEmission()const { return m_emission; }
		const String& GetDiffuseTexture()const { return m_diffuseTexture; }
		const String& GetBumpTexture()const { return m_bumpTexture; }
	
	public:
        String m_name;
        float m_exponent = 0.0f;                        //Ns
        Vector3f m_ambient;                    //Ka
        Vector3f m_diffuse;                    //Kd
        Vector3f m_specular;                   //Ks
        Vector3f m_emission;                   //Ke
        String m_diffuseTexture;              // map_Kd
		String m_bumpTexture;					// map_bump
    };

}