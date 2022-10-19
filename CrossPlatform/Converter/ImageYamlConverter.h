#pragma once
#include "yaml-cpp/yaml.h"
#include "CrossPlatform/Image/Image.h"

namespace YAML {
    using CrossPlatform::Image;
    template<>
    struct convert<Image> {
        static Node encode(const Image& rhs) {
            Node node;
			//YAML::Node node = YAML::Binary(rhs.m_data, rhs.m_height * rhs.m_width);
			node["Data"] = YAML::Binary(rhs.m_data, rhs.m_height * rhs.m_width);
			node["Height"] = rhs.m_height;
			node["Width"] = rhs.m_width;
			node["Channel"] = rhs.m_channel;

            return node;
        }

        static bool decode(const Node& node, Image& rhs) {
			//TODO
			// Need to check
			// Memory leak possibility
			YAML::Binary binary = node["Data"].as<YAML::Binary>();
            rhs.m_data = new unsigned char[binary.size()];
            memcpy(rhs.m_data, binary.data(), binary.size());

			//const_cast<unsigned char*>(binary.data());
            rhs.m_height = node["Height"].as<int>();
            rhs.m_width = node["Width"].as<int>();
            rhs.m_channel = node["Channel"].as<int>();
            return true;
        }
    };
}