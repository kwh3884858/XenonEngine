#pragma once
#include <mutex>

namespace CrossPlatform
{
    class Folder;
}
namespace XenonEngine
{
    using CrossPlatform::Folder;
    class GameObjectWorld;
    class Graphic3D;

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

        void FolderSetter(const Folder* folder) {
            std::lock_guard<std::mutex> lck(m_folderMutex);
            m_folder = folder;
        }

        const Folder* FolderGetter()const
        {
            std::lock_guard<std::mutex> lck(m_folderMutex);
            return m_folder;
        }

    private:
        mutable std::mutex m_mutex;
        const GameObjectWorld* m_gameWorld = nullptr;

        mutable std::mutex m_graphic3DMutex;
        const Graphic3D* m_graphic3D = nullptr;

        mutable std::mutex m_folderMutex;
        const Folder* m_folder = nullptr;
    };

    extern XenonEngine::EngineSyncData* pGlobalSyncData;
}
