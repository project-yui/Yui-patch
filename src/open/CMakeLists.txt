
set(NAME "open")
add_library("${NAME}"
    SHARED
    ../include/init.hh
    ../config/init.cc
    ./open.cc
    )

target_link_libraries("${NAME}" PRIVATE spdlog::spdlog)
target_link_libraries("${NAME}" PRIVATE JsonCpp::JsonCpp)

set_target_properties("${NAME}" PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if(UNIX)
    set_target_properties("${NAME}" PROPERTIES PREFIX "lib" SUFFIX ".so")
endif()
# SET_TARGET_PROPERTIES("${NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\Software\\Fiddler Everywhere 5.10.0\\resources\\app\\out\\WebServer")
