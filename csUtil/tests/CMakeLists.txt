macro(cs_test target sources)
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  target_include_directories(${target} PRIVATE
    ${csUtil_BINARY_DIR}/include
    ${csUtil_SOURCE_DIR}/include
    )
  target_link_libraries(${target} csUtil)
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${csUtil_BINARY_DIR})
endmacro()

### Tests ####################################################################

cs_test(test_stringutil src/test_stringutil.cpp)
