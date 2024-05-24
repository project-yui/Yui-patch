
//
// created by AheadLibEx
// Author:i1tao
// Blog:https://www.cnblogs.com/0xc5
// Github:https://github.com/i1tao/AheadLibEx
// 

#include <cwchar>
#include <spdlog/spdlog.h>
#include <windows.h>
#include <string>
#include <vector>
#include <Shlwapi.h>
#include "../include/hook.hh"
#include "../include/init.hh"
#include <wincrypt.h>
#include <sstream>
#include <iostream>
#include <iomanip>

#pragma comment( lib, "Shlwapi.lib")
#pragma comment(lib, "version.lib")
// #pragma comment(lib, "crypt32.lib")

#pragma comment(linker, "/EXPORT:corehost_initialize=AheadLibEx_corehost_initialize,@1")
#pragma comment(linker, "/EXPORT:corehost_load=AheadLibEx_corehost_load,@2")
#pragma comment(linker, "/EXPORT:corehost_main=AheadLibEx_corehost_main,@3")
#pragma comment(linker, "/EXPORT:corehost_main_with_output_buffer=AheadLibEx_corehost_main_with_output_buffer,@4")
#pragma comment(linker, "/EXPORT:corehost_resolve_component_dependencies=AheadLibEx_corehost_resolve_component_dependencies,@5")
#pragma comment(linker, "/EXPORT:corehost_set_error_writer=AheadLibEx_corehost_set_error_writer,@6")
#pragma comment(linker, "/EXPORT:corehost_unload=AheadLibEx_corehost_unload,@7")

extern "C" 
{
PVOID pfnAheadLibEx_corehost_initialize;
PVOID pfnAheadLibEx_corehost_load;
PVOID pfnAheadLibEx_corehost_main;
PVOID pfnAheadLibEx_corehost_main_with_output_buffer;
PVOID pfnAheadLibEx_corehost_resolve_component_dependencies;
PVOID pfnAheadLibEx_corehost_set_error_writer;
PVOID pfnAheadLibEx_corehost_unload;
}


static HMODULE g_OldModule = NULL;

VOID WINAPI Free()
{
	if (g_OldModule)
	{
		FreeLibrary(g_OldModule);
	}
}


BOOL WINAPI Load()
{
	TCHAR tzPath[MAX_PATH];
	TCHAR tzTemp[MAX_PATH * 2];

	//
	// 这里是否从系统目录或当前目录加载原始DLL
	//
	GetModuleFileName(NULL,tzPath,MAX_PATH); //获取本目录下的
	PathRemoveFileSpec(tzPath);

	// GetSystemDirectory(tzPath, MAX_PATH); //默认获取系统目录的

	lstrcat(tzPath, TEXT("\\hostpolicy.original.dll"));

	g_OldModule = LoadLibrary(tzPath);
	if (g_OldModule == NULL)
	{
		wsprintf(tzTemp, TEXT("无法找到模块 %s,程序无法正常运行"), tzPath);
		MessageBox(NULL, tzTemp, TEXT("AheadLibEx"), MB_ICONSTOP);
	}

	return (g_OldModule != NULL);

}


FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
	FARPROC fpAddress;
	CHAR szProcName[64];
	TCHAR tzTemp[MAX_PATH];

	fpAddress = GetProcAddress(g_OldModule, pszProcName);
	if (fpAddress == NULL)
	{
		if (HIWORD(pszProcName) == 0)
		{
			wsprintfA(szProcName, "#%d", pszProcName);
			pszProcName = szProcName;
		}

		wsprintf(tzTemp, TEXT("无法找到函数 %hs,程序无法正常运行"), pszProcName);
		MessageBox(NULL, tzTemp, TEXT("AheadLibEx"), MB_ICONSTOP);
		ExitProcess(-2);
	}
	return fpAddress;
}

BOOL WINAPI Init()
{
	pfnAheadLibEx_corehost_initialize = GetAddress("corehost_initialize");
	pfnAheadLibEx_corehost_load = GetAddress("corehost_load");
	pfnAheadLibEx_corehost_main = GetAddress("corehost_main");
	pfnAheadLibEx_corehost_main_with_output_buffer = GetAddress("corehost_main_with_output_buffer");
	pfnAheadLibEx_corehost_resolve_component_dependencies = GetAddress("corehost_resolve_component_dependencies");
	pfnAheadLibEx_corehost_set_error_writer = GetAddress("corehost_set_error_writer");
	pfnAheadLibEx_corehost_unload = GetAddress("corehost_unload");
	return TRUE;
}	

DWORD WINAPI ThreadProc(LPVOID lpThreadParameter)
{
	HANDLE hProcess;

	PVOID addr1 = reinterpret_cast<PVOID>(0x00401000);
	BYTE data1[] = { 0x90, 0x90, 0x90, 0x90 };

	hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, GetCurrentProcessId());
	if (hProcess)
	{
		WriteProcessMemory(hProcess, addr1, data1, sizeof(data1), NULL);

		CloseHandle(hProcess);
	}

	return 0;
}

#define MD5LEN 33
std::string CalculateMD5(const std::string& input) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[MD5LEN];
    DWORD hashLen = MD5LEN;
    std::stringstream ss;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        spdlog::error("CryptAcquireContext failed: {}", GetLastError());
        return "";
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
        spdlog::error("CryptCreateHash failed: {}", GetLastError());
        CryptReleaseContext(hProv, 0);
        return "";
    }

    if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(input.c_str()), input.length(), 0)) {
        spdlog::error("CryptHashData failed: ", GetLastError());
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        spdlog::error("CryptGetHashParam failed: ", GetLastError());
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    for (DWORD i = 0; i < hashLen; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return ss.str();
}
std::string GetVersionInfo(HMODULE hModule, const std::string& infoType)
{
    // 获取模块文件路径
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(hModule, path, MAX_PATH);

    // 获取版本信息大小
    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSizeW(path, &handle);
    if (size == 0) {
        return "";
    }

    // 分配缓冲区并获取版本信息
    std::vector<BYTE> buffer(size);
    if (!GetFileVersionInfoW(path, handle, size, buffer.data())) {
        return "";
    }

    // 获取指定的版本信息
    LPVOID value = nullptr;
    UINT len = 0;
    std::string query = "\\StringFileInfo\\040904b0\\" + infoType;
    if (VerQueryValueA(buffer.data(), query.c_str(), &value, &len) && value) {
        return std::string(reinterpret_cast<LPCSTR>(value));
    }

    return "";
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		// 使用示例
		HMODULE hModule = GetModuleHandleA("hostpolicy.dll"); // 替换为你的DLL名称
		std::string fileDescription = GetVersionInfo(hModule, "FileDescription");
		std::string fileVersion = GetVersionInfo(hModule, "FileVersion");
		std::string productVersion = GetVersionInfo(hModule, "ProductVersion");
		std::string productName = GetVersionInfo(hModule, "ProductName");

		std::vector<BYTE> hash;
		std::string data = fileDescription + productName + fileVersion + productVersion + "msojocs->fshefiy";
		// spdlog::info("{}", data.c_str());
 		std::string md5 = CalculateMD5(data);
		md5 = CalculateMD5(md5 + "msojocs->ytdfchg");
		md5 = CalculateMD5(md5 + "msojocs->juguyft");

		if (md5 != "315d198a2b57f17c312f690c1f37231a")
		{
			// spdlog::info("md5: {}", md5.c_str());
			return TRUE;
		}
		if (Load() && Init())
		{
			TCHAR szAppName[MAX_PATH] = TEXT("Fiddler.WebUi.exe");//请修改宿主进程名
			TCHAR szCurName[MAX_PATH];

			GetModuleFileName(NULL, szCurName, MAX_PATH);
			PathStripPath(szCurName);

			//是否判断宿主进程名
			if (StrCmpI(szCurName, szAppName) == 0)
			{
				//启动补丁线程或者其他操作
				HANDLE hThread = CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);
				if (hThread)
				{
					CloseHandle(hThread);
				}
				
			}
			load_configuration();
			create_default_file();
			start_hook();
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}

