
add_library(dbghelp
    SHARED
    ./dbghelp.cc
    ../config/init.cc
    ../hook/hook.cc
    )

if(MSVC)
    target_link_libraries(dbghelp PRIVATE minhook::minhook)
endif()
target_link_libraries(dbghelp PRIVATE spdlog::spdlog)
target_link_libraries(dbghelp PRIVATE JsonCpp::JsonCpp)

set_target_properties(dbghelp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
