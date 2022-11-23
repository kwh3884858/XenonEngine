#pragma once
#include "GameObjectWorldMeta.h"
#include "CrossPlatform/Converter/GameObjectWorldYamlConverter.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include <fstream>
#include <filesystem>
#include "Engine/EngineManager.h"

namespace CrossPlatform
{    
    using namespace XenonEngine;
    using namespace std;
	using namespace std::filesystem;

	void GameObjectWorldMeta::Load()
	{
		if (!m_gameobjectWorld)
		{
			YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
			m_gameobjectWorld = new GameObjectWorld(std::move(config.as<GameObjectWorld>()));
		}

		EngineManager::Get().GetWorldManager().SetCurrentWorld(m_gameobjectWorld);
	}

	void GameObjectWorldMeta::Clear()
	{
		delete m_gameobjectWorld;
		m_gameobjectWorld = nullptr;
	}

	void GameObjectWorldMeta::Save()
	{
		IFileMeta::Save();
		GameObjectWorld* currentWorld = EngineManager::Get().GetWorldManager().GetCurrentWorld();
		// Save As
		if (m_gameobjectWorld != currentWorld)
		{
			Clear();
			//Remove ".world"
			currentWorld->SetWorldName(GetFileHeader().GetFileNameWithoutSuffix());
			m_gameobjectWorld = currentWorld;
		}

		const String& path = GetFileHeader().GetFilePath();
		if (path.Empty())
		{
			return;
		}
		if (!m_gameobjectWorld)
		{
			return;
		}

		{
			ofstream outputStream(path.CString());
			YAML::Emitter out(outputStream);
			out << YAML::Node(*m_gameobjectWorld);
			outputStream.close();
		}
	}

	void GameObjectWorldMeta::Delete()
	{
		IFileMeta::Delete();

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			path worldFile(filePath.CString());
			bool result = remove(worldFile);
			assert(result == true);
		}
	}
}