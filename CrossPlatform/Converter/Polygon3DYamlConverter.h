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
            node["MaterialIndex"] = rhs.m_materialIndex;
            return node;
        }
        static bool decode(const Node& node, Polygon3D::VertexIndex& rhs)
        {
            rhs.m_vertexIndex = node["VertexIndex"].as<int>();
            rhs.m_normalIndex = node["NormalIndex"].as<int>();
            rhs.m_textureCoordinateIndex = node["TextureCoordinateIndex"].as<int>();
            rhs.m_materialIndex = node["MaterialIndex"].as<int>();
            return true;
        }
    };

    template<>
    struct convert<Polygon3D> {
        static Node encode(const Polygon3D& rhs) {
            Node node;
			for (const auto& index : rhs.m_vertexIndex)
			{
				node["VertexIndex"].push_back(index);
			}
            return ;
        }

		static bool decode(const Node& node, Polygon3D& rhs) {
			if (node["VertexIndex"].IsDefined())
			{
				if (node["VertexIndex"].IsSequence())
				{
					for (YAML::const_iterator it = node["VertexIndex"].begin(); it != node["VertexIndex"].end(); ++it) {
						rhs.m_vertexIndex.Add(it->as<Polygon3D::VertexIndex>());
					}
				}
			}
            return true;
        }
    };
}