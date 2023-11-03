#pragma once

namespace XenonEngine
{
	class XenonEngineEditor
	{
	public:
		void Initialize();
		void Update();
		void Shutdown();

		bool IsEditorMode() const;
	protected:
	private:
		bool m_isEditorMode;
		bool m_isEditorModeInLastUpdate;
	};
}
