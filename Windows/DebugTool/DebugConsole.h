#pragma once

#include <wtypes.h>
#include <TCHAR.H>

namespace DebugTool {
    class DebugConsole
    {
    public:
        DebugConsole() = default;
        ~DebugConsole() {
            RetrieveError(_T("MainWindow::Initialize"));
        }

        bool Initialize();
        bool Shutdown();

        void RetrieveError(LPTSTR  lpszFunction);
    protected:
    private:
    };
}
