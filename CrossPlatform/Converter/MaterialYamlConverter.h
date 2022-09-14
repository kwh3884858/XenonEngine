#pragma once
#include "yaml-cpp/yaml.h"
#include "GUIDYamlConverter.h"
#include "CrossPlatform/Material/Material.h"

namespace YAML {
    using CrossPlatform::Material;
    template<>
    struct convert<Material> {
        static Node encode(const Material& rhs) {
            Node node;
			node["m_name"] = rhs.m_name;
			node["exponent"] = rhs.m_exponent;
			node["ambient"] = rhs.m_ambient;
			node["diffuse"] = rhs.m_diffuse;
			node["specular"] = rhs.m_specular;
			node["emission"] = rhs.m_emission;
			node["diffuseTexture"] = rhs.m_diffuseTexture;
			node["bumpTexture"] = rhs.m_bumpTexture;

            return node;
        }

        static bool decode(const Node& node, Material& rhs) {
            rhs.m_name = node["m_name"];
            rhs.m_exponent = node["exponent"];
            rhs.m_ambient = node["ambient"];
            rhs.m_diffuse = node["diffuse"];
            rhs.m_specular = node["specular"];
            rhs.m_emission = node["emission"];
            rhs.m_diffuseTexture = node["diffuseTexture"];
            rhs.m_bumpTexture = node["bumpTexture"];
            return true;
        }
    };
}