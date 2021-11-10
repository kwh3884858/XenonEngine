#pragma once

#include "Engine/Editor/XenonBaseWindow.h"

namespace XenonEngine
{
	class EditorWindowGameObjectWorld : public XenonBaseWindow{
        WINDOW_DECLARE_TYPE(Window_Type::SingleWindow)
    public:
        virtual void Initialize() override {}
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
        virtual const char* GetWindiwName() const override { return m_worldName; }
	private:
        const char* m_worldName = "Empty";
	};
}