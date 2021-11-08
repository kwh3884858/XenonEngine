#include "XenonMaterial.h"
namespace CrossPlatform
{

    XenonMaterial::XenonMaterial(Algorithm::String name, float exponent, MathLab::Vector3f ambient, MathLab::Vector3f diffuse, MathLab::Vector3f specular, MathLab::Vector3f emission, Algorithm::String diffuseTexture) :
        m_name(name),
        m_exponent(exponent),
        m_ambient(ambient),
        m_diffuse(diffuse),
        m_specular(specular),
        m_emission(emission),
        m_diffuseTexture(diffuseTexture)
    {
    }

    XenonMaterial::~XenonMaterial()
    {
    }
}