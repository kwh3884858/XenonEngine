#pragma once

#include <wtypes.h>

namespace DebugTool {
    class DebugConsole
    {
    public:
        DebugConsole() = default;
        ~DebugConsole() = default;

        bool Initialize();
        bool Shutdown();

        void RetrieveError(LPTSTR  lpszFunction);
    protected:
    private:
    };
}
