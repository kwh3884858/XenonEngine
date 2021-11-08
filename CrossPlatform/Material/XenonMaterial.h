#pragma once

#include "Algorithms/String.h"
#include "MathLab/Vector3.h"

namespace CrossPlatform
{
    class XenonMaterial
    {
    public:
        XenonMaterial() {}
        XenonMaterial(Algorithm::String name,float exponent,MathLab::Vector3f ambient,MathLab::Vector3f diffuse,MathLab::Vector3f specular,MathLab::Vector3f emission,Algorithm::String diffuseTexture);
        ~XenonMaterial();

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