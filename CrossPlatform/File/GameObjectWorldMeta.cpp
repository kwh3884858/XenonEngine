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
		GameObjectWorld* currentWorld = EngineManager::Get().GetWorldManager().GetCurrentWorld();
		// Save As
		if (m_gameobjectWorld != currentWorld)
		{
			Clear();
			currentWorld->SetWorldName(GetFileHeader().GetFileName());
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

	void GameObjectWorldMeta::Delete()
	{
		delete m_gameobjectWorld;
		m_gameobjectWorld = nullptr;

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
		{
			String metaFilePath = filePath + ".metadata";
			path worldMetaFile(metaFilePath.CString());
			bool result = remove(worldMetaFile);
			assert(result == true);
		}
	}
}