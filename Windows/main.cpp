#pragma once

#define  IS_PC_PLATFORM true

#if IS_PC_PLATFORM
#include <tchar.h>

#include "MainWindow.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main(int args, char* argv[])
#endif
{
	MainWindow window;
	window.Initialize();
	window.Run();
	window.Shutdown();
	return 0;
}