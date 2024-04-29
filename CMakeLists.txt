cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project("yukihana_patch")

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
message("VCPKG_ROOT: " ${VCPKG_ROOT})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # optional: do not use compiler-specific extensions
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(TARGET_ARCH x64)
if(MSVC)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
    # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
    # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
include("${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

message("LD_LIBRARY_PATH:" ${LD_LIBRARY_PATH})

find_package(minhook CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)


add_library(dbghelp
    SHARED
    ./src/entry.cc
    ./src/init.cc
    ./src/hook/hook.cc
    )
target_link_libraries(dbghelp PRIVATE minhook::minhook)
target_link_libraries(dbghelp PRIVATE spdlog::spdlog)
SET_TARGET_PROPERTIES(dbghelp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\GitHub\\Yukihana\\program")