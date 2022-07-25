macro(cs_test target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  set_target_properties(${target} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )
  target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${csUtil_SOURCE_DIR}/testutils/include
    )
  target_link_libraries(${target} csUtil)
endmacro()

### Tests ####################################################################

cs_test(test_charconv src/test_charconv.cpp)
cs_test(test_dot src/test_dot.cpp)
cs_test(test_endian src/test_endian.cpp ../testutils/include/util.h)
cs_test(test_excd src/test_excd.cpp)
cs_test(test_fileio src/test_fileio.cpp)
cs_test(test_format src/test_format.cpp)
cs_test(test_rawconverter src/test_rawconverter.cpp)
cs_test(test_serial src/test_serial.cpp)
cs_test(test_stringutil src/test_stringutil.cpp)
cs_test(test_textconverter src/test_textconverter.cpp)
cs_test(test_trie src/test_trie.cpp)
