
#include <spdlog/spdlog.h>
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		spdlog::info("libfiddler Dll main!");
		info_check("libfiddler.dll", "4120fc3f6693e5b8634fc5be004aac3b");
	}
	return TRUE;
}


extern "C" __declspec(dllexport) void f() {
	
}