#pragma once
#include "Algorithms/String.h"
#include "Algorithms/Vector.h"
namespace XenonPhysics
{
    class Physics2D;
}
namespace XenonEngine
{
    class GameObject;
    class GameObjectWorld
    {
    public:
        GameObjectWorld(const Algorithm::String& worldName = "Untitled");
        ~GameObjectWorld();
        const GameObjectWorld& operator=(const GameObjectWorld& world) = delete;
        GameObjectWorld* Copy()const;

        const Algorithm::String& GetWorldName()const { return m_worldName; }
        void SetWorldName(const Algorithm::String& name) { m_worldName = name; }

        void AddGameObject(GameObject* const gameobject);
        GameObject* GetGameObject(const Algorithm::String& gameObjectName) const;
		Algorithm::Vector<GameObject*> GetGameObjectList(const Algorithm::String& gameObjectName)const;
		Algorithm::Vector<const Algorithm::String*> GetGameObjectNameList(const Algorithm::String& gameObjectName)const;
        const Algorithm::Vector<GameObject*>& GetWorldObjects()const { return m_worldObjects; }
        
        void Start();
        void Update();
        void Destroy();
    private:
        void ClearMarkForDelete();
        void RenderUpdate();
		void ObjectUpdate();

        Algorithm::String m_worldName;
        Algorithm::Vector<GameObject*> m_worldObjects;
        XenonPhysics::Physics2D* m_physics2D = nullptr;
    };

}