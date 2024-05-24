
enable_language(ASM)
set(ASM_COMPILER "ml64")

set(ASM_SOURCE_FILES ./hostpolicy_jump.asm)
# 添加自定义命令来编译汇编文件
foreach(ASM_FILE ${ASM_SOURCE_FILES})
    get_filename_component(ASM_FILE_NAME ${ASM_FILE} NAME_WE)
    set(OUTPUT_OBJ "${CMAKE_CURRENT_BINARY_DIR}/${ASM_FILE_NAME}.obj")
    add_custom_command(
        OUTPUT ${OUTPUT_OBJ}
        COMMAND ${ASM_COMPILER} /Fo ${OUTPUT_OBJ} /c /Cp ${CMAKE_CURRENT_SOURCE_DIR}/${ASM_FILE}
        DEPENDS ${ASM_FILE}
        COMMENT "Assembling ${ASM_FILE}"
    )
    # 将生成的对象文件添加到列表中，以便后续链接
    list(APPEND OBJECT_FILES ${OUTPUT_OBJ})
endforeach()

set(NAME "hostpolicy")
add_library("${NAME}"
    SHARED
    ./hostpolicy.cc
    ../config/init.cc
    ../hook/hook.cc
    ${OBJECT_FILES}
    )
target_link_libraries("${NAME}" PRIVATE minhook::minhook)
target_link_libraries("${NAME}" PRIVATE spdlog::spdlog)
target_link_libraries("${NAME}" PRIVATE JsonCpp::JsonCpp)

# SET_TARGET_PROPERTIES("${NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "D:\\Software\\Fiddler Everywhere 5.9.0\\resources\\app\\out\\WebServer")
