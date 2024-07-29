#include "../include/init.hh"
#include <dlfcn.h>
#include <filesystem>
// #include <map>
#include <spdlog/spdlog.h>
// #include <string>
// #include <sys/types.h>

// mac: https://stackoverflow.com/questions/34114587/dyld-library-path-dyld-insert-libraries-not-working

#if __APPLE__
#define DYLD_INTERPOSE(_replacment,_replacee) \
__attribute__((used)) static struct{ const void* replacment; const void* replacee; } _interpose_##_replacee \
__attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };
#endif

using std::string;

typedef int (*old_open)(const char *p, int flags, mode_t mode);
typedef FILE* (*old_fopen)(const char *filename, const char* mode);
typedef int (*old_creat)(const char *pathname, mode_t mode);
typedef int (*old_file)(const char *pathname);
typedef FILE *(*old_fdopen)(int fd, const char *mode);
typedef FILE *(*old_freopen)(const char *pathname, const char *mode, FILE *stream);

// typedef int (*CAC_FUNC)(int, int);
#ifdef __linux__
extern "C" int open(const char *file, int flags, mode_t mode) {
#elif __APPLE__
int pOpen(const char *file, int flags, mode_t mode) {
#endif
  printf("open hook\n");
  old_open oopen = (old_open)dlsym(RTLD_NEXT, "open");
  std::string filename(file);
  spdlog::info("full filename: {}", filename.c_str());

  std::filesystem::path p(filename);

  if (p.is_absolute()) {
    // 绝对路径
    spdlog::info("relative filename: {}", filename.c_str());
  }

  if (config.find(filename.c_str()) != config.end()) {
    spdlog::info("File config was found: {}", filename);
    auto directData = config[filename.c_str()];
    if (directData.cur >= directData.start && directData.cur < directData.end) {
      spdlog::info("Redirect for: {}", filename);
      directData.cur++;
      // 文件名

      const char *strTmpPath = directData.target.c_str();
      spdlog::info("Redirect to: {}", strTmpPath);

      return oopen(strTmpPath, flags, mode);

    } else {
      spdlog::info("target: {}, cur: {}, start: {}, end: {}", directData.target,
                   directData.cur, directData.start, directData.end);
    }

  } else {
    spdlog::info("Can not find file config: {}", filename.c_str());
  }

  return oopen(file, flags, mode);
}
#if __APPLE__
DYLD_INTERPOSE(pOpen, open);
#endif
// extern "C" FILE* fopen(const char* file, const char* mode) {
//   printf("fopen hook\n");
//   old_fopen oopen = (old_fopen)dlsym(RTLD_NEXT, "fopen");
//   std::string filename(file);
//   spdlog::info("full filename: {}", filename.c_str());

//   std::filesystem::path p(filename);

//   if (p.is_absolute()) {
//     // 绝对路径
//     spdlog::info("relative filename: {}", filename.c_str());
//   }

//   if (config.find(filename.c_str()) != config.end()) {
//     spdlog::info("File config was found: {}", filename);
//     auto directData = config[filename.c_str()];
//     if (directData.cur >= directData.start && directData.cur < directData.end) {
//       spdlog::info("Redirect for: {}", filename);
//       directData.cur++;
//       // 文件名

//       const char *strTmpPath = directData.target.c_str();
//       spdlog::info("Redirect to: {}", strTmpPath);

//       return oopen(strTmpPath, mode);

//     } else {
//       spdlog::info("target: {}, cur: {}, start: {}, end: {}", directData.target,
//                    directData.cur, directData.start, directData.end);
//     }

//   } else {
//     spdlog::info("Can not find file config: {}", filename.c_str());
//   }

//   return oopen(file, mode);
// }
// extern "C" int creat(const char *file, mode_t mode) {
//   printf("creat hook\n");
//   old_creat oopen = (old_creat)dlsym(RTLD_NEXT, "creat");
//   std::string filename(file);
//   spdlog::info("full filename: {}", filename.c_str());

//   std::filesystem::path p(filename);

//   if (p.is_absolute()) {
//     // 绝对路径
//     spdlog::info("relative filename: {}", filename.c_str());
//   }

//   if (config.find(filename.c_str()) != config.end()) {
//     spdlog::info("File config was found: {}", filename);
//     auto directData = config[filename.c_str()];
//     if (directData.cur >= directData.start && directData.cur < directData.end) {
//       spdlog::info("Redirect for: {}", filename);
//       directData.cur++;
//       // 文件名

//       const char *strTmpPath = directData.target.c_str();
//       spdlog::info("Redirect to: {}", strTmpPath);

//       return oopen(strTmpPath, mode);

//     } else {
//       spdlog::info("target: {}, cur: {}, start: {}, end: {}", directData.target,
//                    directData.cur, directData.start, directData.end);
//     }

//   } else {
//     spdlog::info("Can not find file config: {}", filename.c_str());
//   }

//   return oopen(file, mode);
// }
// extern "C" int file(const char *file) {
//   printf("file hook\n");
//   old_file oopen = (old_file)dlsym(RTLD_NEXT, "file");
//   std::string filename(file);
//   spdlog::info("full filename: {}", filename.c_str());

//   std::filesystem::path p(filename);

//   if (p.is_absolute()) {
//     // 绝对路径
//     spdlog::info("relative filename: {}", filename.c_str());
//   }

//   if (config.find(filename.c_str()) != config.end()) {
//     spdlog::info("File config was found: {}", filename);
//     auto directData = config[filename.c_str()];
//     if (directData.cur >= directData.start && directData.cur < directData.end) {
//       spdlog::info("Redirect for: {}", filename);
//       directData.cur++;
//       // 文件名

//       const char *strTmpPath = directData.target.c_str();
//       spdlog::info("Redirect to: {}", strTmpPath);

//       return oopen(strTmpPath);

//     } else {
//       spdlog::info("target: {}, cur: {}, start: {}, end: {}", directData.target,
//                    directData.cur, directData.start, directData.end);
//     }

//   } else {
//     spdlog::info("Can not find file config: {}", filename.c_str());
//   }

//   return oopen(file);
// }
// extern "C" FILE* fdopen(int fd, const char* mode) {
//   printf("fdopen hook\n");
//   old_fdopen oopen = (old_fdopen)dlsym(RTLD_NEXT, "fdopen");

//   return oopen(fd, mode);
// }
// extern "C" FILE* freopen(const char *pathname, const char *mode, FILE *stream) {
//   printf("freopen hook\n");
//   old_freopen oopen = (old_freopen)dlsym(RTLD_NEXT, "freopen");

//   return oopen(pathname, mode, stream);
// }
extern "C" int open64(const char *file, int flags, mode_t mode) {
  printf("open64 hook\n");
  old_open oopen = (old_open)dlsym(RTLD_NEXT, "open64");
  std::string filename(file);
  spdlog::info("full filename: {}", filename.c_str());

  std::filesystem::path p(filename);

  if (p.is_absolute()) {
    // 绝对路径
    spdlog::info("relative filename: {}", filename.c_str());
  }

  if (config.find(filename.c_str()) != config.end()) {
    spdlog::info("File config was found: {}", filename);
    auto directData = config[filename.c_str()];
    if (directData.cur >= directData.start && directData.cur < directData.end) {
      spdlog::info("Redirect for: {}", filename);
      directData.cur++;
      // 文件名

      const char *strTmpPath = directData.target.c_str();
      spdlog::info("Redirect to: {}", strTmpPath);

      return oopen(strTmpPath, flags, mode);

    } else {
      spdlog::info("target: {}, cur: {}, start: {}, end: {}", directData.target,
                   directData.cur, directData.start, directData.end);
    }

  } else {
    spdlog::info("Can not find file config: {}", filename.c_str());
  }

  return oopen(file, flags, mode);
}

__attribute__((constructor)) void init() {
  spdlog::info("libopen loading...");
  load_configuration();
  create_default_file();
  spdlog::info("libopen loaded...");
}