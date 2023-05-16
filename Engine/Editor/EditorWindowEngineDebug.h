#pragma once

#include "Engine/Editor/XenonBaseWindow.h"

namespace XenonEngine
{
	class EditorWindowEngineDebug : public XenonBaseWindow{
        WINDOW_DECLARE_NAME("Engine Debug")
        WINDOW_DECLARE_TYPE(Window_Type::SingleWindow)
    public:
        virtual void Initialize() override;
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
	private:
	};

}