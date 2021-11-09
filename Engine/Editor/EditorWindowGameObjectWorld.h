#pragma once

#include "Engine/Editor/XenonBaseWindow.h"

namespace XenonEngine
{
	class EditorWindowGameObjectWorld : public XenonBaseWindow{
        WINDOW_DECLARE("HeavenGateWindowBgmTable", Window_Type::MainWindow)
    public:
        virtual void Initialize() override {}
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
	private:
	};
}