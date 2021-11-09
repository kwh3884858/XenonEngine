#pragma once

#include "Engine/Editor/XenonBaseWindow.h"

namespace XenonEngine
{
	class EditorWindowFileDatabase : public XenonBaseWindow{
        WINDOW_DECLARE("EditorWindowFileDatabase", Window_Type::MainWindow)
    public:
        virtual void Initialize() override {}
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
	private:
	};

}