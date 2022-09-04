//  StringYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "yaml-cpp/yaml.h"

#include "Vector3YamlConverter.h"
#include "CrossPlatform/Converter/GUIDYamlConverter.h"

namespace YAML {
    using CrossPlatform::Polygon3D;

    template<>
    struct convert<Polygon3D::VertexIndexs>
    {
        static Node encode(const Polygon3D::VertexIndexs& rhs)
        {
            Node node;
			node["VertexIndex"] = rhs.m_vertexIndex;
			node["NormalIndex"] = rhs.m_normalIndex;
			node["TextureCoordinateIndex"] = rhs.m_textureCoordinateIndex;
            return node;
        }
        static bool decode(const Node& node, Polygon3D::VertexIndexs& rhs)
        {
            rhs.m_vertexIndex = node["VertexIndex"].as<int>();
            rhs.m_normalIndex = node["NormalIndex"].as<int>();
            rhs.m_textureCoordinateIndex = node["TextureCoordinateIndex"].as<int>();
            return true;
        }
    }

    template<>
    struct convert<Polygon3D> {
        static Node encode(const Polygon3D& rhs) {
            Node node;
            node["NumOfIndex"] = rhs.m_numOfIndex;
            for (int i = 0; i < rhs.m_numOfIndex; i++)
            {
                node["VertexIndexList"]->push_back( *(rhs.m_vertexIndexList[i]) );
            }
            node["NumOfVertex"] = rhs.m_numOfVertex;
			for (int i = 0; i < rhs.m_numOfVertex; i++)
			{
				node["VertexList"]->push_back(*(rhs.m_vertexList[i]));
			}
            node["NumOfNormal"] = rhs.m_numOfNormal;
			for (int i = 0; i < rhs.m_numOfNormal; i++)
			{
				node["NormalList"]->push_back(*(rhs.m_normalList[i]));
			}
            node["NumOfTextureCoordinate"] = rhs.m_numOfTextureCoordinate;
			for (int i = 0; i < rhs.m_numOfTextureCoordinate; i++)
			{
				node["NormalList"]->push_back(*(rhs.m_textureCoordinate[i]));
			}
            node["Material"] = rhs.m_material;
            return ;
        }

        static bool decode(const Node& node, Polygon3D& rhs) {
            rhs = node["NumOfIndex"].as<int>();
            return true;
        }
    };
}