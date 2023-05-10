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

	XenonEngine::GameObjectWorld* GameObjectWorldMeta::Instantiate()
	{
		// TODO: Save meta GUID into instance.
		YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
		GameObjectWorld* gameobjectWorld = new GameObjectWorld(std::move(config.as<GameObjectWorld>()));
		return gameobjectWorld;
	}

	void GameObjectWorldMeta::Clear()
	{
		//delete m_gameobjectWorld;
		//m_gameobjectWorld = nullptr;
	}

	void GameObjectWorldMeta::Save(const XenonObject* /*data*/ /*= nullptr*/)
	{
		IFileMeta::Save();
		GameObjectWorld* const currentWorld = EngineManager::Get().GetWorldManager().GetCurrentWorld();
		// Save As
		currentWorld->SetWorldName(GetFileHeader().GetFileNameWithoutSuffix());

		const String& path = GetFileHeader().GetFilePath();
		if (path.Empty())
		{
			return;
		}

		{
			ofstream outputStream(path.CString());
			YAML::Emitter out(outputStream);
			out << YAML::Node(*currentWorld);
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