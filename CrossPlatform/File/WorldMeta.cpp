#pragma once
#include "WorldMeta.h"
#include "CrossPlatform/Converter/GameObjectWorldYamlConverter.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include <fstream>

namespace CrossPlatform
{    
    using namespace XenonEngine;
    using namespace std;
    WorldMeta::~WorldMeta()
    {
        delete m_gameobjectWorld;
        m_gameobjectWorld = nullptr;
    }

    XenonEngine::GameObjectWorld* WorldMeta::GetGameObjectWorld()
    {
        if (!m_gameobjectWorld)
        {
            YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
            m_gameobjectWorld = config.as<GameObjectWorld>().Copy();
        }
        return m_gameobjectWorld;
    }

    void WorldMeta::SaveGameObjectWorld()const
    {
        const String& path = GetFileHeader().GetFilePath();
        if (path.Empty())
        {
            return;
        }
        if (!m_gameobjectWorld)
        {
            return;
        }

        String metaFilePath = path + ".metadata";
        {
            ofstream outputStream(metaFilePath.CString());
            YAML::Emitter out(outputStream);
            out << YAML::Node(GetFileHeader());
            outputStream.close();
        }
        {
            ofstream outputStream(path.CString());
            YAML::Emitter out(outputStream);
            out << YAML::Node(*m_gameobjectWorld);
            outputStream.close();
        }
    }
}