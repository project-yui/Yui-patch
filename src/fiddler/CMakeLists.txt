
if(MSVC)
    set(INFO_CHECK "../info_check.cc")
    set(MY_VERSIONINFO_RC "${CMAKE_BINARY_DIR}/libfiddler.rc")
    configure_file("./libfiddler.rc.in"
                   "${MY_VERSIONINFO_RC}")
endif()
add_library(libfiddler
    SHARED
    ./fiddler.cc
    ${INFO_CHECK}
    ${MY_VERSIONINFO_RC}
    )

target_link_libraries(libfiddler PRIVATE spdlog::spdlog)

set_target_properties(libfiddler PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if(APPLE)
    set_target_properties(libfiddler PROPERTIES PREFIX "lib" SUFFIX ".dylib")
elseif(UNIX)
    set_target_properties(libfiddler PROPERTIES PREFIX "" SUFFIX ".so")
endif()
# SET_TARGET_PROPERTIES(libfiddler PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\Software\\Fiddler Everywhere 5.10.0")