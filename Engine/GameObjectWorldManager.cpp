#include "GameObjectWorldManager.h"
#include "Algorithms/String.h"
#include "Engine/GameObject.h"
#include "Engine/GameObjectWorld.h"
#include <filesystem>

#include "yaml-cpp/yaml.h"
#include "EngineManager.h"
#include "CrossPlatform/File/WorldMeta.h"
#include "CrossPlatform/Converter/GameObjectWorldYamlConverter.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include <fstream>


namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;

    bool GameObjectWorldManager::Initialize()
    {
        if (!m_currentWorld)
        {
            CreateGameWorld("Untitled");
        }
        return true;
    }

    bool GameObjectWorldManager::Shutdown() 
    {
        m_currentWorld = nullptr;
        for (int i = 0; i < m_worlds.Count(); i++)
        {
            delete m_worlds[i];
        }
        return true;
    }

    GameObjectWorldManager::~GameObjectWorldManager()
    {
        assert(m_currentWorld == nullptr);
        assert(m_worlds.Count() == 0);
    }

    void GameObjectWorldManager::LoadWorld(const Algorithm::String& worldMetaFilePath)
    {
        //path metaFilePath(worldMetaFilePath.CString());
        //if (!exists(metaFilePath))
        //{
        //    return;
        //}
        //if (metaFilePath.extension() != "metadata")
        //{
        //    return;
        //}
        //YAML::Node config = YAML::LoadFile(metaFilePath.generic_string());
        //FileHeader header = config.as<FileHeader>();
        //WorldMeta* metaFile = (WorldMeta*)EngineManager::Get().GetFileDatabase().GetFile(header.GetGUID());

        //AddGameWorld(metaFile->GetGameObjectWorld());
    }

    void GameObjectWorldManager::SaveWorld(const Algorithm::String& savePath) const
    {

        //xg::Guid guid = xg::newGuid();
        //WorldMeta worldMeta(FileHeader(FileType::FileTypeWorld, savePath, guid));
        //{

        //}
        //{
        //    ofstream outputStream(savePath.CString());
        //    YAML::Emitter out(outputStream);
        //    out << YAML::Node(*m_currentWorld);
        //    outputStream.close();
        //}
    }

    //void GameObjectWorldManager::AddGameObject(GameObject* gameobject)
    //{
    //    if (m_currentWorld != nullptr)
    //    {
    //        m_currentWorld->AddGameObject(gameobject);
    //    }
    //    for (int i = 0; i < m_worlds.Count(); i++)
    //    {
    //    }
    //     m_worlds.Add(gameobject); 
    //}

    //XenonEngine::GameObject* GameObjectWorldManager::GetGameObject(const Algorithm::String& GameObjectName) const
    //{
    //    for (int i = 0; i < m_worlds.Count(); i++)
    //    {
    //        if (m_worlds[i]->GetName() == GameObjectName)
    //        {
    //            return m_worlds[i];
    //        }
    //    }
    //}

    void GameObjectWorldManager::Update()
    {
        assert(m_currentWorld != nullptr);
        m_currentWorld->Update();
    }

    XenonEngine::GameObjectWorld*const GameObjectWorldManager::CreateGameWorld(const Algorithm::String& worldName)
    {
        GameObjectWorld* newWorld = new GameObjectWorld(worldName);
        AddGameWorld(newWorld);
        return newWorld;
    }

    GameObjectWorld*const GameObjectWorldManager::GetCurrentWorld() const
    {
        assert(m_currentWorld != nullptr);
        return m_currentWorld;
    }

    void GameObjectWorldManager::AddGameWorld(GameObjectWorld* world)
    {
        m_worlds.Add(world);
        if (m_currentWorld == nullptr)
        {
            m_currentWorld = world;
        }
    }

}