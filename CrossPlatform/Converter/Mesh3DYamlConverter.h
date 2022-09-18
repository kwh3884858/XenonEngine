#pragma once
#include "yaml-cpp/yaml.h"

#include "GUIDYamlConverter.h"
#include "Vector3YamlConverter.h"

#include "Engine/Component/Mesh3D.h"

namespace YAML {
    using XenonEngine::Mesh3D;
    template<>
    struct convert<Mesh3D> {
        static Node encode(const Mesh3D& rhs) {
            Node node;
			for (const auto& polygon:rhs.m_polygons)
			{
				node["Polygon"].push_back(polygon);
			}
			for (const auto& vertex : rhs.m_vertexs)
			{
				node["Vertex"].push_back(vertex);
			}
			for (const auto& normal : rhs.m_normals)
			{
				node["Normal"].push_back(normal);
			}
			for (const auto& uv : rhs.m_uv)
			{
				node["UV"].push_back(uv);
			}
			for (const auto& material : rhs.m_materials)
			{
				node["Material"].push_back(material);
			}
            return node;
        }

        static bool decode(const Node& node, Mesh3D& rhs) {
			if (node["Polygon"].IsDefined())
			{
				if (node["Polygon"].IsSequence())
				{
					for (YAML::const_iterator it = node["Polygon"].begin(); it != node["Polygon"].end(); ++it) {
						rhs.m_polygons.Add(it->as<xg::Guid>());
					}
				}
			}
			if (node["Vertex"].IsDefined())
			{
				if (node["Vertex"].IsSequence())
				{
					for (YAML::const_iterator it = node["Vertex"].begin(); it != node["Vertex"].end(); ++it) {
						rhs.m_vertexs.Add(it->as<MathLab::Vector3f>());
					}
				}
			}
			if (node["Normal"].IsDefined())
			{
				if (node["Normal"].IsSequence())
				{
					for (YAML::const_iterator it = node["Normal"].begin(); it != node["Normal"].end(); ++it) {
						rhs.m_normals.Add(it->as<MathLab::Vector3f>());
					}
				}
			}
			if (node["UV"].IsDefined())
			{
				if (node["UV"].IsSequence())
				{
					for (YAML::const_iterator it = node["UV"].begin(); it != node["UV"].end(); ++it) {
						rhs.m_uv.Add(it->as<MathLab::Vector2f>());
					}
				}
			}
			if (node["Material"].IsDefined())
			{
				if (node["Material"].IsSequence())
				{
					for (YAML::const_iterator it = node["Material"].begin(); it != node["Material"].end(); ++it) {
						rhs.m_materials.Add(it->as<xg::Guid>());
					}
				}
			}
            return true;
        }
    };
}