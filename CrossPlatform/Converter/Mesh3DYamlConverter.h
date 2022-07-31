#pragma once
#include "yaml-cpp/yaml.h"
#include "GUIDYamlConverter.h"
#include "Engine/Component/Mesh3D.h"

namespace YAML {
    using XenonEngine::Mesh3D;
    template<>
    struct convert<Mesh3D> {
        static Node encode(const Mesh3D& rhs) {
            Node node;
			for (const auto& polygon:rhs.m_polygons)
			{
				node["polygons"].push_back(polygon);
			}
			for (const auto& material : rhs.m_materials)
			{
				node["materials"].push_back(material);
			}
            return node;
        }

        static bool decode(const Node& node, Mesh3D& rhs) {
			if (node["polygons"].IsDefined())
			{
				if (node["polygons"].IsSequence())
				{
					for (YAML::const_iterator it = node["polygons"].begin(); it != node["polygons"].end(); ++it) {
						rhs.m_polygons.Add(it->as<xg::Guid>());
					}
				}
			}
			if (node["materials"].IsDefined())
			{
				if (node["materials"].IsSequence())
				{
					for (YAML::const_iterator it = node["materials"].begin(); it != node["materials"].end(); ++it) {
						rhs.m_materials.Add(it->as<xg::Guid>());
					}
				}
			}
            return true;
        }
    };
}