//  StringYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "yaml-cpp/yaml.h"

//#include "Vector3YamlConverter.h"
#include "CrossPlatform/Converter/GUIDYamlConverter.h"

namespace YAML {
    using CrossPlatform::Polygon3D;

    template<>
    struct convert<Polygon3D::VertexIndex>
    {
        static Node encode(const Polygon3D::VertexIndex& rhs)
        {
            Node node;
            node["VertexIndex"] = rhs.m_vertexIndex;
            node["NormalIndex"] = rhs.m_normalIndex;
            node["TextureCoordinateIndex"] = rhs.m_textureCoordinateIndex;
            //node["MaterialIndex"] = rhs.m_materialIndex;
            return node;
        }
        static bool decode(const Node& node, Polygon3D::VertexIndex& rhs)
        {
            rhs.m_vertexIndex = node["VertexIndex"].as<int>();
            rhs.m_normalIndex = node["NormalIndex"].as<int>();
            rhs.m_textureCoordinateIndex = node["TextureCoordinateIndex"].as<int>();
            //rhs.m_materialIndex = node["MaterialIndex"].as<int>();
            return true;
        }
    };

	template<>
	struct convert<Polygon3D::TriangleIndex>
	{
		static Node encode(const Polygon3D::TriangleIndex& rhs)
		{
			Node node;
			for (const auto& vertex : rhs.m_vertex)
			{
				node["Vertex"].push_back(vertex);
			}
			node["MaterialIndex"] = rhs.m_materialIndex;
			return node;
		}
		static bool decode(const Node& node, Polygon3D::TriangleIndex& rhs)
		{
			if (node["Vertex"].IsDefined())
			{
				if (node["Vertex"].IsSequence())
				{
					YAML::const_iterator it = node["Vertex"].begin();
					rhs.m_vertex[0] = it->as<Polygon3D::VertexIndex>();
					rhs.m_vertex[1] = it->as<Polygon3D::VertexIndex>();
					rhs.m_vertex[2] = it->as<Polygon3D::VertexIndex>();
					//for (YAML::const_iterator it = node["Vertex"].begin(); it != node["Vertex"].end(); ++it) {
					//	rhs.m_vertex.Add(it->as<Polygon3D::VertexIndex>());
					//}
				}
			}
			rhs.m_materialIndex = node["MaterialIndex"].as<int>();
			return true;
		}
	};

    template<>
    struct convert<Polygon3D> {
        static Node encode(const Polygon3D& rhs) {
            Node node;
			for (const auto& index : rhs.m_triangle)
			{
				node["Triangle"].push_back(index);
			}
            return node;
        }

		static bool decode(const Node& node, Polygon3D& rhs) {
			if (node["Triangle"].IsDefined())
			{
				if (node["Triangle"].IsSequence())
				{
					for (YAML::const_iterator it = node["Triangle"].begin(); it != node["Triangle"].end(); ++it) {
						rhs.m_triangle.Add(it->as<Polygon3D::VertexIndex>());
					}
				}
			}
            return true;
        }
    };
}