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
        GameObjectWorld(const Algorithm::String& worldName);
        ~GameObjectWorld();
        const GameObjectWorld& operator=(const GameObjectWorld& world) = delete;

        void AddGameObject(GameObject* const gameobject);
        GameObject* GetGameObject(const Algorithm::String& GameObjectName) const;

        void Start();
        void Update();
        void Destroy();
    private:
        void ClearMarkForDelete();
        void RenderUpdate();
		void ObjectUpdate();

        Algorithm::String m_worldName;
        Algorithm::Vector<GameObject*> m_worldObjects;
        XenonPhysics::Physics2D* m_physics2D;
    };

}