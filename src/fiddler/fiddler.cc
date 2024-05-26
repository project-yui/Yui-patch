
#include <spdlog/spdlog.h>
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		spdlog::info("libfiddler Dll main!");
		info_check("libfiddler.dll", "aeb9377b0c499bbd3cbc827148b50948");
	}
	return TRUE;
}


extern "C" __declspec(dllexport) void f() {
	
}