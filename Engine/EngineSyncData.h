#pragma once
#include <mutex>

namespace XenonEngine
{
    class GameObjectWorld;
    class Graphic3D;
    class FileDatabase;

    class EngineSyncData
    {
    public:
        void WorldSetter(const GameObjectWorld* world) {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_gameWorld = world;
        }

        const GameObjectWorld* WorldGetter()const
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            return m_gameWorld;
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
        const GameObjectWorld* m_gameWorld = nullptr;

        mutable std::mutex m_graphic3DMutex;
        const Graphic3D* m_graphic3D = nullptr;

        mutable std::mutex m_databaseMutex;
        const FileDatabase* m_database = nullptr;
    };

    extern XenonEngine::EngineSyncData* pGlobalSyncData;
}
