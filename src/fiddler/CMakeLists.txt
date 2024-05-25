
if(MSVC)
    set(MY_VERSIONINFO_RC "${CMAKE_BINARY_DIR}/VersionInfo.rc")
    configure_file("./VersionInfo.rc.in"
                   "${MY_VERSIONINFO_RC}")
endif()
add_library(libfiddler
    SHARED
    ./fiddler.cc
    ../info_check.cc
    ${MY_VERSIONINFO_RC}
    )

target_link_libraries(libfiddler PRIVATE spdlog::spdlog)

# SET_TARGET_PROPERTIES(libfiddler PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\Software\\Fiddler Everywhere 5.10.0")