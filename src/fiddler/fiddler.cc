
#include <spdlog/spdlog.h>
#ifdef WIN32
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"
#endif

#ifdef WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		spdlog::info("libfiddler Dll main!");
		// info_check("libfiddler.dll", "b7a70b0ac3884c4f5c15dffc5d90b8e6");
	}
	return TRUE;
}


extern "C" __declspec(dllexport) void initialize() {
	
}
extern "C" __declspec(dllexport) void f() {
	
}
#endif

#if defined(__linux__) || __APPLE__
void initialize() {
	
}
void f() {
	
}
#endif