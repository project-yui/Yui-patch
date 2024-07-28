#include "../include/init.hh"
#include <cstring>

#ifdef __WIN__
#include <Windows.h>
#include <direct.h>
#elif defined(__linux__)
#include <sys/stat.h>
#include <unistd.h>
#define MAX_PATH 255
#endif

#include <filesystem>
#include <fstream>
#include <ios>
#include <spdlog/spdlog.h>
#include <json/json.h>

#if defined(__linux__)
std::map<std::string, RedirectInfo> config;
#endif

inline bool exists_test (const std::string& name) {
#ifdef __WIN__
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
#elif defined(__linux__)
  return !access(name.c_str(), F_OK);
#endif
}

void load_configuration()
{
    spdlog::info("configuration load start.");
    // Here, using a specialized Builder, we discard comments and
    // record errors as we parse.
    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    Json::Value root;   // 'root' will contain the root value after parsing.
    std::ifstream input("./patch.json", std::ios_base::in);
    if (input.is_open())
    {
        std::string errs;
        bool ok = Json::parseFromStream(rbuilder, input, &root, &errs);
        if (!ok)return;

        if (root.isObject())
        {
            std::map<std::string, std::string> t;
            auto keys = root.getMemberNames();
            for (auto& key : keys) {
                spdlog::info("key: {}", key);

                RedirectInfo info;
                auto item = root[key];
                
                auto target = item["target"];
                if (target.isString())
                {
                    info.target = target.asString();
                }
                auto content = item["content"];
                if (content.isString())
                {
                    info.content = content.asString();
                }
                auto cur = item["cur"];
                if (cur.isInt())
                {
                    info.cur = cur.asInt();
                }
                auto start = item["start"];
                if (start.isInt())
                {
                    info.start = start.asInt();
                }
                auto end = item["end"];
                if (end.isInt())
                {
                    info.end = end.asInt();
                }
                std::filesystem::path p(key);
                if (p.is_relative())
                {
                    #ifdef __WIN__
                    char * cwd = _getcwd(NULL, 0);
                    #elif defined(__linux__)
                    char * cwd = getcwd(NULL, 0);
                    #endif
                    spdlog::info("reaolve relative path: {}, cwd: {}", key.c_str(), cwd);
                    auto p1 = std::filesystem::canonical(key);
                    key = p1.string();
                    spdlog::info("absolute path: {}", key.c_str());
                }
                spdlog::info("add");
                config.emplace(key, info);
                spdlog::info("add ok");
            }
        }
    }
    spdlog::info("configuration load completed.");

}

void create_default_file()
{
    spdlog::info("default file create start.");
    char	strTmpPath[MAX_PATH];
    #ifdef __WIN__
	GetTempPath(sizeof(strTmpPath), strTmpPath);
    #elif defined(__linux__)
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
    strcpy(strTmpPath, temp_dir.c_str());
    #endif
    spdlog::info("temp path: {}", strTmpPath);

    spdlog::info("config size: {}", config.size());
    for (auto& cfg: config) {
        auto& redirectData = cfg.second;
        std::filesystem::path p(redirectData.target);
        if (p.is_absolute())
        {
            spdlog::info("absolute");
            // 绝对路径
            if (exists_test(redirectData.target))
            {
                continue;
            }
            std::ofstream fs(redirectData.target, std::ios_base::out);
            
            if (fs.is_open())
            {
                fs.write(redirectData.content.c_str(), redirectData.content.length());
                fs.flush();
                fs.close();
            }
            else {
                spdlog::error("[Absolute] Failed to open file: {}", redirectData.target);
            }
        
        }
        else
        {
            spdlog::info("relative: {}", redirectData.target);
            // 相对路径
            if (exists_test(redirectData.target))
            {
                spdlog::info("exists skip");
                continue;
            }
            std::ofstream fs(redirectData.target, std::ios_base::out);
            if (fs.is_open())
            {
                fs.write(redirectData.content.c_str(), redirectData.content.length());
                fs.flush();
                fs.close();
            }
            else {
                spdlog::warn("[Relative1] Failed to open file: {}", redirectData.target);
                std::filesystem::path t(strTmpPath);
                t.append(redirectData.target);
                redirectData.target = t.string();
                if (exists_test(redirectData.target))
                {
                    spdlog::info("exists: {}", redirectData.target);
                    continue;
                }
                std::ofstream fsn(redirectData.target, std::ios_base::out);
                if (fsn.is_open())
                {
                    fsn.write(redirectData.content.c_str(), redirectData.content.length());
                    fsn.flush();
                    fsn.close();
                }
                else {
                    spdlog::error("[Relative2] Failed to open file: {}", redirectData.target);
                }
            }
        }
    }
    spdlog::info("default file create completed.");
}