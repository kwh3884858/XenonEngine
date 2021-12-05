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
        Material(const String& name,float exponent,const MathLab::Vector3f& ambient,const MathLab::Vector3f& diffuse,const MathLab::Vector3f& specular,const MathLab::Vector3f& emission,const Algorithm::String& diffuseTexture);
        ~Material();

		const String& GetName()const { return m_name; }
		const float& GetExponent()const { return m_exponent; }
		const Vector3f& GetAmbient()const { return m_ambient; }
		const Vector3f& GetDiffuse()const { return m_diffuse; }
		const Vector3f& GetSpecular()const { return m_specular; }
		const Vector3f& GetEmission()const { return m_emission; }
		const String& GetDiffuseTexture()const { return m_diffuseTexture; }
    private:
        String m_name;
        float m_exponent = 0.0f;                        //Ns
        Vector3f m_ambient;                    //Ka
        Vector3f m_diffuse;                    //Kd
        Vector3f m_specular;                   //Ks
        Vector3f m_emission;                   //Ke
        String m_diffuseTexture;              // map_Kd
    };

}