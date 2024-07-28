
//
// created by AheadLibEx
// Author:i1tao
// Blog:https://www.cnblogs.com/0xc5
// Github:https://github.com/i1tao/AheadLibEx
// 

#include <cwchar>
#include <spdlog/spdlog.h>
#include <windows.h>
#include <Shlwapi.h>
#include "../include/hook.hh"
#include "../include/init.hh"
#include "../include/info_check.hh"
#include <wincrypt.h>

#pragma comment( lib, "Shlwapi.lib")
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


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		info_check("hostpolicy.dll", "5330086346dc3b928495cb22d296a6ef");
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
		else {
			spdlog::error("load failed...");
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}

