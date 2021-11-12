#pragma once

#include "Engine/Editor/XenonBaseWindow.h"
#include "Library/ImGuiFileDialog/ImGuiFileDialog.h"

namespace XenonEngine
{
	class EditorWindowFileDatabase : public XenonBaseWindow{
        WINDOW_DECLARE_NAME("File Database")
        WINDOW_DECLARE_TYPE(Window_Type::SingleWindow)
    public:
        virtual void Initialize() override;
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
	private:
        ImGuiFileDialog m_contentBrowser;
        ImGuiFileDialog m_fileImporter;
	};

}