#include "Minhook.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fileapi.h>
#include <minwindef.h>
#include <psapi.h>
#include "../include/nt.hh"
#include "../include/const.hh"
#include "../include/hook.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winnt.h>
#include <winuser.h>


static def_CreateFileW Org_CreateFileW = NULL;
static def_ReadFile Org_ReadFile = NULL;
static int i = 0;

HANDLE WINAPI Hk_CreateFileW(
    _In_           LPCWSTR                lpFileName,
    _In_           DWORD                 dwDesiredAccess,
    _In_           DWORD                 dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_           DWORD                 dwCreationDisposition,
    _In_           DWORD                 dwFlagsAndAttributes,
    _In_opt_ HANDLE                hTemplateFile
) {
    char path[MAX_PATH];
    size_t len;
    wcstombs_s(&len, path, MAX_PATH, lpFileName, wcslen(lpFileName));
    std::string filename(path, len);
    printf("filename: %s\n", filename.c_str());

    if (filename.find("app_launcher\\index.js") != std::string::npos) {
        if (i < 1){
            i++;
            // 文件名
            // MessageBoxA(NULL, filename.c_str(), NULL, 0);

            char	strTmpPath[MAX_PATH];
            GetTempPath(sizeof(strTmpPath), strTmpPath);
            strcat_s(strTmpPath, DEFAULT_INDEX);

            int cap = (strlen(strTmpPath) + 1) * sizeof(wchar_t);
            wchar_t *defaultIndex = (wchar_t *)malloc(cap);
            size_t retlen = 0;
            
            
            errno_t err = mbstowcs_s(&retlen, defaultIndex, cap / sizeof(wchar_t), strTmpPath, _TRUNCATE);
            // MessageBoxA(NULL, "convert over\0", NULL, 0);
            if (err == 0) {
                HANDLE ret = Org_CreateFileW(defaultIndex, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                free(defaultIndex);
                return ret;
            }

            // MessageBoxA(NULL, msg, NULL, 0);
            // BOOL readResult = ReadFile(ret, msg, 20, NULL, NULL);
            // sprintf(msg, "read result: %d\0", readResult);
            // MessageBoxA(NULL, msg, NULL, 0);
            // 释放
            
        }
        
    }

    return Org_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


BOOL WINAPI Hk_ReadFile(
    _In_                HANDLE       hFile,
    _Out_               LPVOID       lpBuffer,
    _In_                DWORD        nNumberOfBytesToRead,
    _Out_opt_     LPDWORD      lpNumberOfBytesRead,
    _In_opt_ LPOVERLAPPED lpOverlapped
) {
    return Org_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}


void start_hook() {
    if (MH_Initialize() != MH_OK) {
        MessageBoxA(nullptr, "MH Init Error!", "ERROR", MB_ICONERROR | MB_OK);
        exit(1);
    }
    if (MH_CreateHook(&CreateFileW, &Hk_CreateFileW, reinterpret_cast<LPVOID*>(&Org_CreateFileW)) != MH_OK) {
        MessageBoxA(nullptr, "MH Hook CreateFileW failed!", "ERROR", MB_ICONERROR | MB_OK);
        exit(1);
    }
    // if (MH_CreateHook(&ReadFile, &Hk_ReadFile, reinterpret_cast<LPVOID*>(&Org_ReadFile)) != MH_OK) {
    //     MessageBoxA(nullptr, "MH Hook CreateFileW failed!", "ERROR", MB_ICONERROR | MB_OK);
    //     exit(1);
    // }
    
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        MessageBoxA(nullptr, "MH enable all hooks failed!", "ERROR", MB_ICONERROR | MB_OK);
        exit(1);
    }
}