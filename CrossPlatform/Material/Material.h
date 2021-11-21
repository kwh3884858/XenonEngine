#pragma once

#include "Algorithms/String.h"
#include "MathLab/Vector3.h"

namespace CrossPlatform
{
    class Material
    {
    public:
        Material(const Algorithm::String& name,float exponent,const MathLab::Vector3f& ambient,const MathLab::Vector3f& diffuse,const MathLab::Vector3f& specular,const MathLab::Vector3f& emission,const Algorithm::String& diffuseTexture);
        ~Material();

    private:
        Algorithm::String m_name;

        float m_exponent = 0.0f;                        //Ns
        MathLab::Vector3f m_ambient;                    //Ka
        MathLab::Vector3f m_diffuse;                    //Kd
        MathLab::Vector3f m_specular;                   //Ks
        MathLab::Vector3f m_emission;                   //Ke
        Algorithm::String m_diffuseTexture;              // map_Kd
    };

}