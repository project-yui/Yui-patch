add_executable(test_fiddler
    ./fiddler.cc
)
set_target_properties(test_fiddler PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_link_libraries(test_fiddler PUBLIC dl)