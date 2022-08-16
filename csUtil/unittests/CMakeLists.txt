macro(cs_unittest target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  set_target_properties(${target} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )
  if(MSVC)
    target_compile_options(${target}
      PRIVATE /source-charset:UTF-8 /execution-charset:UTF-8 /validate-charset
      )
  endif()
  target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${csUtil_SOURCE_DIR}/3rdparty/catch2/include
    PRIVATE ${csUtil_SOURCE_DIR}/testutils/include
    )
  target_link_libraries(${target} csUtilHeaders csUtilLib)
endmacro()

### Unit Tests ###############################################################

cs_unittest(unittest_endian src/unittest_endian.cpp src/unittest_endian_impl.cpp src/unittest_endian_main.cpp)
cs_unittest(unittest_fileio src/unittest_fileio.cpp src/unittest_fileio_main.cpp)
cs_unittest(unittest_flags src/unittest_flags.cpp src/unittest_flags_main.cpp)
cs_unittest(unittest_numeric src/unittest_numeric.cpp src/unittest_numeric_main.cpp)
cs_unittest(unittest_simd src/unittest_simd.cpp src/unittest_simd_main.cpp)
cs_unittest(unittest_statistics src/unittest_statistics.cpp src/unittest_statistics_main.cpp)
cs_unittest(unittest_stringutil src/unittest_stringutil.cpp src/unittest_stringutil_main.cpp)
if(ENABLE_UNICODE)
  cs_unittest(unittest_textconverter src/unittest_textconverter.cpp src/unittest_textconverter_main.cpp)
endif()
cs_unittest(unittest_trie src/unittest_trie.cpp src/unittest_trie_main.cpp)
