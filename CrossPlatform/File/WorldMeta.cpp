#pragma once
#include "WorldMeta.h"
#include "CrossPlatform/Converter/GameObjectWorldYamlConverter.h"

namespace CrossPlatform
{    
    using namespace XenonEngine;
    WorldMeta::~WorldMeta()
    {
        delete m_gameobjectWorld;
        m_gameobjectWorld = nullptr;
    }

    XenonEngine::GameObjectWorld* WorldMeta::GetGameObject()
    {
        if (!m_gameobjectWorld)
        {
            YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
            m_gameobjectWorld = config.as<GameObjectWorld>().Copy();
        }
        return m_gameobjectWorld;
    }

}