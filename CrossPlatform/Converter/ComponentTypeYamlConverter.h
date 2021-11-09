#pragma once
#include "yaml-cpp/yaml.h"
#include "Engine/Component/IComponent.h"

namespace YAML {
    using XenonEngine::IComponent;

    template<>
    struct convert<IComponent::IComponentType> {
        static Node encode(const IComponent::IComponentType& rhs) {
            return Node((int)rhs);
        }

        static bool decode(const Node& node, IComponent::IComponentType& rhs) {
            rhs = static_cast<IComponent::IComponentType>(node.as<int>());
            return true;
        }
    };
}
