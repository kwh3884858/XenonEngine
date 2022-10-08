//  WorldMeta.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
    class FileDatabase;
    class GameObjectWorld;
}

namespace CrossPlatform
{    
    class GameObjectWorldMeta :public IFileMeta
    {
    public:
        friend class XenonEngine::FileDatabase;
        GameObjectWorldMeta(const FileHeader& header) :IFileMeta(header) {}
        virtual ~GameObjectWorldMeta()override = default;

		// Load into memory
		virtual void Load() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save() override;

		// Delete data file from hard drive
		virtual void Delete() override;
		//void SetGameObjectWorld(XenonEngine::GameObjectWorld* world) { m_gameobjectWorld = world; }

    protected:
    private:
		//XenonEngine::GameObjectWorld* GetGameObjectWorld();
        XenonEngine::GameObjectWorld* m_gameobjectWorld = nullptr;
    };

}