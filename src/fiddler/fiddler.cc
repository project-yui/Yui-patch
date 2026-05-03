
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <fstream>
#include <string>
#include <vector>
#ifdef WIN32
#include <windows.h>
#include <winnt.h>
#include "../include/info_check.hh"
#endif
#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
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
bool restartCurrentProcess() {
	std::string cmd = GetCommandLineA();
	STARTUPINFOA si{};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi{};

	// CreateProcess may modify command line buffer.
	std::vector<char> cmdBuffer(cmd.begin(), cmd.end());
	cmdBuffer.push_back('\0');

	BOOL ok = CreateProcessA(
		NULL,
		cmdBuffer.data(),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);

	if (ok) {
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}
	return false;
}
#endif

#ifdef __linux__
bool restartCurrentProcess() {
	char exePath[PATH_MAX] = {0};
	ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
	if (len <= 0) {
		return false;
	}
	exePath[len] = '\0';

	pid_t pid = fork();
	if (pid < 0) {
		return false;
	}
	if (pid == 0) {
		execl(exePath, exePath, static_cast<char*>(NULL));
		_exit(127);
	}
	return true;
}
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
	if (fixPackageJson()) {
		MessageBoxA(NULL, "package.json was fixed, please restart", "libfiddler", MB_OK | MB_ICONINFORMATION);
		if (restartCurrentProcess()) {
			ExitProcess(0);
		}
	}
}
extern "C" __declspec(dllexport) void f() {
	if (fixPackageJson()) {
		MessageBoxA(NULL, "package.json was fixed, please restart", "libfiddler", MB_OK | MB_ICONINFORMATION);
		if (restartCurrentProcess()) {
			ExitProcess(0);
		}
	}
}
#endif

#if defined(__linux__) || __APPLE__
extern "C" {
void initialize() {
	spdlog::info("libfiddler initialize");
	if (fixPackageJson()) {
		spdlog::info("package.json was fixed, please restart");
#ifdef __linux__
		if (restartCurrentProcess()) {
			std::exit(0);
		}
#endif
	}
}
void f() {
	spdlog::info("libfiddler f");
	if (fixPackageJson()) {
		spdlog::info("package.json was fixed, please restart");
#ifdef __linux__
		if (restartCurrentProcess()) {
			std::exit(0);
		}
#endif
	}
}
}
#endif
