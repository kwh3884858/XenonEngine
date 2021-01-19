#include "DebugConsole.h"

#include <Windows.h> //AllocConsole and FreeConsole
#include <strsafe.h> 
//#include <afxwin.h>

namespace DebugTool {

    bool DebugConsole::Initialize() {
        if (!AllocConsole()) {
            //AfxMessageBox("Failed to create the console!", MB_ICONEXCLAMATION);
            return false;
        }
        return true;
    }

    bool DebugConsole::Shutdown() {
        if (!FreeConsole()) {
            //AfxMessageBox("Could not free the console!");
            return false;
        }
        return true;
    }

    void DebugConsole::RetrieveError(LPTSTR lpszFunction) {
        // Retrieve the system error message for the last-error code

        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        // Display the error message and exit the process

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"),
            lpszFunction, dw, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
        ExitProcess(dw);
    }
}