#pragma once
#include "yaml-cpp/yaml.h"
#include "Engine/GameObjectWorld.h"
#include "GameObjectYamlConverter.h"

namespace YAML {
    using XenonEngine::GameObjectWorld;
    using XenonEngine::GameObject;
    template<>
    struct convert<GameObjectWorld> {
        static Node encode(const GameObjectWorld& rhs) {
            Node node;
            node["Name"] = rhs.GetWorldName();
            const Algorithm::Vector<GameObject*>& allObjects = rhs.GetWorldObjects();
            for (int i = 0; i < allObjects.Count(); i++)
            {
                node["WorldObjects"].push_back((*allObjects[i]));
            }
            return node;
        }

        static bool decode(const Node& node, GameObjectWorld& rhs) {
			if (node["Name"].IsDefined())
			{
				rhs.SetWorldName(node["Name"].as<Algorithm::String>());
			}
            else
            {
                return false;
            }

            if (node["WorldObjects"].IsDefined())
            {
                if (node["WorldObjects"].IsSequence())
                {
                    for (YAML::const_iterator it = node["WorldObjects"].begin(); it != node["WorldObjects"].end(); ++it) {
                        rhs.AddGameObject(std::move(it->as<GameObject>()));
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }
    };
}