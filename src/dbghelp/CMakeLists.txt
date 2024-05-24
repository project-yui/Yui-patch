
add_library(dbghelp
    SHARED
    ./dbghelp.cc
    ../config/init.cc
    ../hook/hook.cc
    )

target_link_libraries(dbghelp PRIVATE minhook::minhook)
target_link_libraries(dbghelp PRIVATE spdlog::spdlog)
target_link_libraries(dbghelp PRIVATE JsonCpp::JsonCpp)

# SET_TARGET_PROPERTIES(dbghelp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\GitHub\\Yukihana\\program")