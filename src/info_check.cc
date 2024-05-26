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
    std::string fileDescription = GetVersionInfo(hModule, "FileDescription");
    std::string fileVersion = GetVersionInfo(hModule, "FileVersion");
    std::string productVersion = GetVersionInfo(hModule, "ProductVersion");
    std::string productName = GetVersionInfo(hModule, "ProductName");
    spdlog::info("name: {}, desc: {}, ver: {}", productName.c_str(), fileDescription.c_str(), fileVersion.c_str());

    std::vector<BYTE> hash;
    std::string data = fileDescription + productName + fileVersion + productVersion + "msojocs->fshefiy";
    std::string md5 = CalculateMD5(data);
    md5 = CalculateMD5(md5 + "msojocs->ytdfchg");
    md5 = CalculateMD5(md5 + "msojocs->juguyft");

    if (md5 != result)
    {
        // spdlog::info("md5: {}", md5.c_str());
        exit(-1);
    }
    spdlog::info("info_check done");
}