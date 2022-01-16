#pragma once
#include "yaml-cpp/yaml.h"
#include "Engine/Component/Camera3DController.h"

namespace YAML {
    template<>
    struct convert<XenonEngine::Camera3DController> {
		static Node encode(const XenonEngine::Camera3DController& rhs) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, XenonEngine::Camera3DController& rhs) {
			return true;
		}
    };
}