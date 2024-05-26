
#include <spdlog/spdlog.h>
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		spdlog::info("libfiddler Dll main!");
		info_check("libfiddler.dll", "f5d389d86ff863cc78fbd4527cfbaa51");
	}
	return TRUE;
}


extern "C" __declspec(dllexport) void f() {
	
}