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
    class WorldMeta :public IFileMeta
    {
    public:
        friend class XenonEngine::FileDatabase;
        WorldMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeWorld); }
        virtual ~WorldMeta()override;
		void Load() override;
		void Save() override;
		void Delete() override;
        XenonEngine::GameObjectWorld* GetGameObjectWorld();
		void SetGameObjectWorld(XenonEngine::GameObjectWorld* world) { m_gameobjectWorld = world; }

		//void DeleteGameObjectWorld();
    protected:
    private:
		//void SaveGameObjectWorld()const;
        XenonEngine::GameObjectWorld* m_gameobjectWorld = nullptr;
    };

}