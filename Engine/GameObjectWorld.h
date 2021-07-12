//  GameObejctWorld.h
//  XenonEngine
//
//  Created by whkong on 2021-7-12.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "Algorithms/Vector.h"
#include "Algorithms/String.h"
#include "CrossPlatform/XenonManager.h"

namespace XenonEngine
{
    class GameObject;

    class GameObjectWorld :public CrossPlatform::XenonManager<GameObjectWorld>
    {
    public:
        //initilize function, take the place of constructor
        virtual bool Initialize()override;

        //destroy function, take the  place of deconstructor
        virtual bool Shutdown() override;
        virtual ~GameObjectWorld() override;

        void AddGameObject(GameObject* gameobject) { m_world.Add(gameobject); }
        GameObject* GetGameObject(const Algorithm::String& GameObjectName) const;

    private:
        Algorithm::Vector<GameObject*> m_world;
    };

}
