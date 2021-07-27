#pragma once
#include "Algorithms/String.h"
#include "Algorithms/Vector.h"
namespace XenonEngine
{
    class GameObject;
    class GameObjectWorld
    {
    public:
        GameObjectWorld(const Algorithm::String& worldName);
        ~GameObjectWorld();
        const GameObjectWorld& operator=(const GameObjectWorld& world) = delete;

        void AddGameObject(GameObject* const gameobject);
        GameObject* GetGameObject(const Algorithm::String& GameObjectName) const;

        void Update();
    private:
        void RenderUpdate();

        Algorithm::String m_worldName;
        Algorithm::Vector<GameObject*> m_worldObjects;
    };

}