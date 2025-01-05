# Locate all test files
file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(_test IN ITEMS ${TEST_FILES})
    get_filename_component(_test_name ${_test} NAME_WE)
    add_executable(${_test_name} ${_test})

    target_link_libraries(${_test_name} PRIVATE AbstractSyntaxTree)

    set_target_properties(${_test_name} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    target_include_directories(${_test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    add_dependencies(${_test_name} AbstractSyntaxTree)

    # Add test command
    add_test(NAME ${_test_name} COMMAND $<TARGET_FILE:${_test_name}>)
endforeach()