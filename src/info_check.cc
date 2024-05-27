#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <wincrypt.h>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "crypt32.lib")


std::wstring GetVersionInfo(HMODULE hModule, const std::wstring& infoType)
{
    // 获取模块文件路径
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(hModule, path, MAX_PATH);

    // 获取版本信息大小
    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSizeW(path, &handle);
    if (size == 0) {
        return L"";
    }

    // 分配缓冲区并获取版本信息
    std::vector<BYTE> buffer(size);
    if (!GetFileVersionInfoW(path, handle, size, buffer.data())) {
        return L"";
    }

    // 获取指定的版本信息
    LPVOID value = nullptr;
    UINT len = 0;
    std::wstring query = L"\\StringFileInfo\\040904b0\\" + infoType;
    if (VerQueryValueW(buffer.data(), query.c_str(), &value, &len) && value) {
        return std::wstring(reinterpret_cast<LPCWSTR>(value));
    }

    return L"";
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

void info_check(const char* name, const char* result)
{
    spdlog::info("info_check");
    // 使用示例
    HMODULE hModule = GetModuleHandleA(name); // 替换为你的DLL名称
    std::wstring fileDescription = GetVersionInfo(hModule, L"FileDescription");
    std::wstring fileVersion = GetVersionInfo(hModule, L"FileVersion");
    std::wstring productVersion = GetVersionInfo(hModule, L"ProductVersion");
    std::wstring productName = GetVersionInfo(hModule, L"ProductName");
    std::wstring t = fileDescription + productName + fileVersion + productVersion;
    char *temp = (char *)malloc(sizeof(char) * t.length() * 2);
    memset(temp, 0, sizeof(char) * t.length() * 2);
    int ret = WideCharToMultiByte(65001, 0, t.c_str(), t.length(), temp, t.length() * 2, "\0", 0);
    if (ret >= 0)
    {
        spdlog::info("length: {}, data: {}",t.length(), temp);
    }

    std::vector<BYTE> hash;
    std::string data = temp + std::string("msojocs->fshefiy");
    free(temp);
    std::string md5 = CalculateMD5(data);
    md5 = CalculateMD5(md5 + "msojocs->ytdfchg");
    md5 = CalculateMD5(md5 + "msojocs->juguyft");

    if (md5 != result)
    {
        spdlog::info("md5: {}", md5.c_str());
        exit(-1);
    }
    spdlog::info("info_check done");
}