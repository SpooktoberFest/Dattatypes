
# Add Source Files
file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

foreach(test_file IN LISTS TEST_FILES)
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin
    )
    target_link_libraries(${test_name} PRIVATE Dattatypes)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()