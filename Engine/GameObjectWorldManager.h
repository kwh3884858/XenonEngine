//  GameObejctWorld.h
//  XenonEngine
//
//  Created by whkong on 2021-7-12.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "Algorithms/Vector.h"
#include "Algorithms/String.h"

namespace XenonEngine
{
    class GameObject;
    class GameObjectWorld;

    class GameObjectWorldManager final
    {
    public:
        //initilize function, take the place of constructor
        bool Initialize();

        //destroy function, take the  place of deconstructor
        bool Shutdown() ;
        ~GameObjectWorldManager() ;

        void LoadWorld(const Algorithm::String& worldMetaFilePath);
        void SaveWorld(const Algorithm::String& savePath)const;

        void Update();
        GameObjectWorld*const CreateGameWorld(const Algorithm::String& worldName);
        GameObjectWorld*const GetCurrentWorld() const;
        void AddGameWorld(GameObjectWorld* world);
    private:
        GameObjectWorld* m_currentWorld = nullptr;
        Algorithm::Vector<GameObjectWorld*> m_worlds;
    };

}
