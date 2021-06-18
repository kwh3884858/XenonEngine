#pragma once

#ifdef WIN32 // is windows.
#ifdef _DEBUG // is debug.

#define XenonEngine_Debug 1;

#else         // not debug.

//#define XenonEngine_Debug 0;

#endif // _DEBUG
#endif // WIN32