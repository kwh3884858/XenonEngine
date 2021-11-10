//  WorldMeta.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
    class GameObjectWorld;
}

namespace CrossPlatform
{    
    class WorldMeta :public IFileMeta
    {
    public:
        WorldMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeWorld); }
        virtual ~WorldMeta()override;
        XenonEngine::GameObjectWorld* GetGameObject();
    protected:
    private:
        XenonEngine::GameObjectWorld* m_gameobjectWorld = nullptr;
    };

}