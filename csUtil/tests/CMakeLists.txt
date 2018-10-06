include_directories(
  ${csUtil_BINARY_DIR}/include
  ${csUtil_SOURCE_DIR}/include
  )

### csStringUtil #############################################################

add_executable(test_stringutil
  src/test_stringutil.cpp
  )

format_output_name(test_stringutil "test_stringutil")

target_link_libraries(test_stringutil csUtil)

set_target_properties(test_stringutil PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${csUtil_BINARY_DIR}
  )
