#include "Minhook.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fileapi.h>
#include <map>
#include <minwindef.h>
#include <psapi.h>
#include "../include/nt.hh"
#include "../include/hook.hh"
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winnt.h>
#include <winuser.h>
#include <direct.h>
#include <filesystem>
#include "../include/redirect.hh"


static def_CreateFileW Org_CreateFileW = NULL;
static def_ReadFile Org_ReadFile = NULL;

std::map<std::string, RedirectInfo> config;
//  = {
//     {
//             std::string("program\\resources\\app\\app_launcher\\index.js"),
//             {
//                 std::string("index.js"),
//                 "require('./launcher.node').load('external_index', module);",
//                 0, 0, 1
//             }
//         },
// };

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
    spdlog::info("full filename: {}", filename.c_str());
    
    std::filesystem::path p(filename);

    if (p.is_absolute())
    {
        // 绝对路径
        // 获取当前目录
        char * cwd = _getcwd(NULL, 0);
        spdlog::info("cwd: {}", cwd);
        // if (filename.find("\\\\?\\") == 0) {
        //     filename.replace(0, 5, "");
        // }
        filename.replace(0, strlen(cwd) + 1, "");
        spdlog::info("relative filename: {}", filename.c_str());
    }

    if (config.find(filename.c_str()) != config.end())
    {
        spdlog::info("File config was found: {}", filename);
        auto directData = config[filename.c_str()];
        if (directData.cur >= directData.start && directData.cur < directData.end)
        {
            spdlog::info("Redirect for: {}", filename);
            directData.cur++;
            // 文件名

            const char	*strTmpPath = directData.target.c_str();

            int cap = (strlen(strTmpPath) + 1) * sizeof(wchar_t);
            wchar_t *defaultIndex = (wchar_t *)malloc(cap);
            size_t retlen = 0;
            
            errno_t err = mbstowcs_s(&retlen, defaultIndex, cap / sizeof(wchar_t), strTmpPath, _TRUNCATE);

            if (err == 0) {
                HANDLE ret = Org_CreateFileW(defaultIndex, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                free(defaultIndex);
                return ret;
            }

            // BOOL readResult = ReadFile(ret, msg, 20, NULL, NULL);
            spdlog::info("read result: {}\n", err);
            // 释放
            free(defaultIndex);
        }
        else {
            spdlog::info("target: {}, cur: {}, start: {}, end: {}", directData.target, directData.cur, directData.start, directData.end);
        }
        
    }
    else {
        spdlog::info("Can not find file config: {}", filename.c_str());
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