#pragma once

#include "Engine/Editor/XenonBaseWindow.h"
#include "Algorithms/String.h"
#include "Library/ImGuiFileDialog/ImGuiFileDialog.h"
namespace XenonEngine
{
    class GameObjectWorld;
    class GameObject;
	class EditorWindowGameObjectWorld : public XenonBaseWindow{
        WINDOW_DECLARE_TYPE(Window_Type::SingleWindow)
    public:
        virtual void Initialize() override {}
        virtual void Shutdown() override {}

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
        virtual const char* GetWindiwName() const override { return m_worldName.CString(); }
	private:
        void ContextMenu(GameObjectWorld* world, GameObject* go = nullptr)const;

        Algorithm::String m_worldName = "World Editor";
        ImGuiFileDialog m_saveDialog;
        ImGuiFileDialog m_loadDialog;
	};
}