
if(MSVC)
    set(INFO_CHECK "../info_check.cc")
    set(MY_VERSIONINFO_RC "${CMAKE_BINARY_DIR}/libfiddler.rc")
    configure_file("./libfiddler.rc.in"
                   "${MY_VERSIONINFO_RC}")
endif()
add_library(fiddler
    SHARED
    ./fiddler.cc
    ${INFO_CHECK}
    ${MY_VERSIONINFO_RC}
    )

target_link_libraries(fiddler PRIVATE spdlog::spdlog)

set_target_properties(fiddler PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if(MSVC)
    set_target_properties(fiddler PROPERTIES PREFIX "" SUFFIX ".dll")
elseif(APPLE)
    set_target_properties(fiddler PROPERTIES PREFIX "" SUFFIX ".dylib")
elseif(UNIX)
    set_target_properties(fiddler PROPERTIES PREFIX "" SUFFIX ".so")
endif()
# SET_TARGET_PROPERTIES(fiddler PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\Software\\Fiddler Everywhere 5.17.0")