#pragma once

#include "Engine/Editor/XenonBaseWindow.h"
#include "Algorithms/String.h"
namespace XenonEngine
{
	class EditorWindowGameObjectWorld : public XenonBaseWindow{
        WINDOW_DECLARE_TYPE(Window_Type::SingleWindow)
    public:
        virtual void Initialize() override {}
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
        virtual const char* GetWindiwName() const override { return m_worldName.CString(); }
	private:
        Algorithm::String m_worldName = "Empty";
	};
}