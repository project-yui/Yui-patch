#include "./include/init.hh"
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <ios>
#include <spdlog/spdlog.h>

inline bool exists_test (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}
void create_default_file()
{
    char	strTmpPath[MAX_PATH];
	GetTempPath(sizeof(strTmpPath), strTmpPath);
    spdlog::info("temp path: {}", strTmpPath);

    for (auto& cfg: config) {
        auto redirectData = cfg.second;
        std::filesystem::path p(redirectData.target);
        if (p.is_absolute())
        {
            spdlog::info("absolute");
            // 绝对路径
            if (exists_test(redirectData.target))
            {
                return;
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
                return;
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
                    return;
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
}