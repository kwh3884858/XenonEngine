#include "XenonEngineEdtior.h"
//#include "CrossPlatform/File/FolderMeta.h"
#include "CrossPlatform/XenonKey.h"
#include "IO/InputSystem.h"

namespace XenonEngine
{

	void XenonEngineEditor::Initialize()
	{
		m_isEditorMode = true;
		m_isEditorModeInLastUpdate = true;
	}

	void XenonEngineEditor::Update()
	{
		if (m_isEditorModeInLastUpdate != m_isEditorMode)
		{
			m_isEditorModeInLastUpdate = m_isEditorMode;
		}

		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
			InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_P))
		{
			m_isEditorMode = !m_isEditorMode;
		}
	}

	void XenonEngineEditor::Shutdown()
	{
		m_isEditorMode = true;
		m_isEditorModeInLastUpdate = true;
	}

	bool XenonEngineEditor::IsEditorMode() const
	{
		return m_isEditorMode && m_isEditorModeInLastUpdate == m_isEditorMode;
	}

}