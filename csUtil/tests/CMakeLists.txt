macro(cs_test target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  target_include_directories(${target} PRIVATE
    ${csUtil_BINARY_DIR}/include
    ${csUtil_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
  target_link_libraries(${target} csUtil)
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${csUtil_BINARY_DIR})
endmacro()

### Tests ####################################################################

cs_test(test_charconv src/test_charconv.cpp)
cs_test(test_endian src/test_endian.cpp src/data.cpp include/data.h)
cs_test(test_flags src/test_flags.cpp)
cs_test(test_format src/test_format.cpp)
cs_test(test_rawconverter src/test_rawconverter.cpp)
cs_test(test_stringutil src/test_stringutil.cpp)
cs_test(test_textconverter src/test_textconverter.cpp)
cs_test(test_trie src/test_trie.cpp)
