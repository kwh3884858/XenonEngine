#pragma once
#include <mutex>

namespace XenonEngine
{
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

    private:
        mutable std::mutex m_mutex;
        const GameObjectWorld* m_gameWorld = nullptr;

        mutable std::mutex m_graphic3DMutex;
        const Graphic3D* m_graphic3D = nullptr;
    };

    extern XenonEngine::EngineSyncData* pGlobalSyncData;
}
