#pragma once
#include "yaml-cpp/yaml.h"
#include "GUIDYamlConverter.h"
#include "Vector3YamlConverter.h"

#include "CrossPlatform/Material/Material.h"

namespace YAML {
    using CrossPlatform::Material;
    template<>
    struct convert<Material> {
        static Node encode(const Material& rhs) {
            Node node;
			node["Name"] = rhs.m_name;
			node["Exponent"] = rhs.m_exponent;
			node["Ambient"] = rhs.m_ambient;
			node["Diffuse"] = rhs.m_diffuse;
			node["Specular"] = rhs.m_specular;
			node["Emission"] = rhs.m_emission;
			node["DiffuseTexture"] = rhs.m_diffuseTexture;
			node["BumpTexture"] = rhs.m_bumpTexture;

            return node;
        }

        static bool decode(const Node& node, Material& rhs) {
            rhs.m_name = node["Name"].as<Algorithm::String>();
            rhs.m_exponent = node["Exponent"].as<float>();
            rhs.m_ambient = node["Ambient"].as<MathLab::Vector3f>();
            rhs.m_diffuse = node["Diffuse"].as<MathLab::Vector3f>();
            rhs.m_specular = node["Specular"].as<MathLab::Vector3f>();
            rhs.m_emission = node["Emission"].as<MathLab::Vector3f>();
            rhs.m_diffuseTexture = node["DiffuseTexture"].as<xg::Guid>();
            rhs.m_bumpTexture = node["BumpTexture"].as<xg::Guid>();
            return true;
        }
    };
}