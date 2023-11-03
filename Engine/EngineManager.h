#pragma once

#include "CrossPlatform/XenonManager.h"
#include "Engine/GameObjectWorldManager.h"
#include "Engine/FileDatabase.h"
#include "Engine/XenonEngineEdtior.h"

namespace XenonEngine
{

    class EngineManager :public CrossPlatform::XenonManager<EngineManager>
    {
    public:
        //initilize function, take the place of constructor
        virtual bool Initialize()override;

        //destroy function, take the  place of deconstructor
        virtual bool Shutdown() override;
        virtual ~EngineManager() override;

        void Update();

        GameObjectWorldManager& GetWorldManager() { return m_worldManager; }
        FileDatabase& GetFileDatabase() { return m_fileDatabase; }
		XenonEngineEditor& GetXenonEngineEditor() { return m_xenonEngineEditor; }
    private:
        GameObjectWorldManager m_worldManager;
        FileDatabase m_fileDatabase;
		XenonEngineEditor m_xenonEngineEditor;
    };

}
