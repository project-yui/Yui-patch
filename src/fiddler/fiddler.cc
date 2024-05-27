
#include <spdlog/spdlog.h>
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		spdlog::info("libfiddler Dll main!");
		info_check("libfiddler.dll", "b7a70b0ac3884c4f5c15dffc5d90b8e6");
	}
	return TRUE;
}


extern "C" __declspec(dllexport) void f() {
	
}