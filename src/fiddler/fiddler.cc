
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <fstream>
#include <string>
#ifdef WIN32
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"
#endif

bool fixPackageJson() {
	const char* filepath = "resources/app/package.json";
	std::ifstream in(filepath, std::ios::binary);
	if (in)
	{
		std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();

		const std::string from = "main.original.js";
		const std::string to = "main.js";
		size_t pos = 0;
		bool changed = false;
		while ((pos = content.find(from, pos)) != std::string::npos)
		{
			content.replace(pos, from.size(), to);
			pos += to.size();
			changed = true;
		}

		if (changed)
		{
			std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
			if (out)
			{
				out.write(content.data(), static_cast<std::streamsize>(content.size()));
			}
			return true;
		}
	}
	return false;
}
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
	if (fixPackageJson()) {
		MessageBoxA(NULL, "package.json was fixed, please restart", "libfiddler", MB_OK | MB_ICONINFORMATION);
		exit(1);
	}
}
extern "C" __declspec(dllexport) void f() {
	if (fixPackageJson()) {
		MessageBoxA(NULL, "package.json was fixed, please restart", "libfiddler", MB_OK | MB_ICONINFORMATION);
		exit(1);
	}
}
#endif

#if defined(__linux__) || __APPLE__
extern "C" {
void initialize() {
	spdlog::info("libfiddler initialize");
	if (fixPackageJson()) {
		spdlog::info("package.json was fixed, please restart");
	}
}
void f() {
	spdlog::info("libfiddler f");
	if (fixPackageJson()) {
		spdlog::info("package.json was fixed, please restart");
	}
}
}
#endif
