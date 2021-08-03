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
    class GameObjectWorld;

    class GameObjectWorldManager :public CrossPlatform::XenonManager<GameObjectWorldManager>
    {
    public:
        //initilize function, take the place of constructor
        virtual bool Initialize()override;

        //destroy function, take the  place of deconstructor
        virtual bool Shutdown() override;
        virtual ~GameObjectWorldManager() override;

        void Update();
        GameObjectWorld*const CreateGameWorld(const Algorithm::String& worldName);
        GameObjectWorld*const GetCurrentWorld() const;
    private:
        GameObjectWorld* m_currentWorld = nullptr;
        Algorithm::Vector<GameObjectWorld*> m_worlds;
    };

}
