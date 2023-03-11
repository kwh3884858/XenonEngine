#pragma once
#include <mutex>

namespace XenonEngine
{
    class GameObjectWorldManager;
    class Graphic3D;
    class FileDatabase;

    class EngineSyncData
    {
    public:
        void WorldManagerSetter(GameObjectWorldManager* world) {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_worldManager = world;
        }

        GameObjectWorldManager* WorldManagerGetter()const
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            return m_worldManager;
        }

        void Graphic3DSetter(const Graphic3D* world) {
            std::lock_guard<std::mutex> lck(m_graphic3DMutex);
            m_graphic3D = world;
        }

        const Graphic3D* Graphic3DGetter()const
        {
            std::lock_guard<std::mutex> lck(m_graphic3DMutex);
            return m_graphic3D;
        }

        void DatabaseSetter(const FileDatabase* database) {
            std::lock_guard<std::mutex> lck(m_databaseMutex);
            m_database = database;
        }

        const FileDatabase* DatabaseGetter()const
        {
            std::lock_guard<std::mutex> lck(m_databaseMutex);
            return m_database;
        }

    private:
        mutable std::mutex m_mutex;
        GameObjectWorldManager* m_worldManager = nullptr;

        mutable std::mutex m_graphic3DMutex;
        const Graphic3D* m_graphic3D = nullptr;

        mutable std::mutex m_databaseMutex;
        const FileDatabase* m_database = nullptr;
    };

    extern XenonEngine::EngineSyncData* pGlobalSyncData;
}
