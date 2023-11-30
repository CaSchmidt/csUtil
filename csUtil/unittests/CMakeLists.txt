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
  target_link_libraries(${target} csUtil)
endmacro()

### Unit Tests ###############################################################

file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA1LongMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA1ShortMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA224LongMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA224ShortMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA256LongMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA256ShortMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA384LongMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA384ShortMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA512LongMsg.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/SHA/SHA512ShortMsg.rsp
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/Word-List.txt
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata
)

cs_unittest(unittest_aes src/unittest_aes.cpp src/unittest_aes_main.cpp)
cs_unittest(unittest_bit src/unittest_bit.cpp src/unittest_bit_main.cpp)
cs_unittest(unittest_bufferedreader src/unittest_bufferedreader.cpp src/unittest_bufferedreader_main.cpp)
cs_unittest(unittest_bytearray src/unittest_bytearray.cpp src/unittest_bytearray_main.cpp)
cs_unittest(unittest_concurrent src/unittest_concurrent.cpp src/unittest_concurrent_main.cpp)
cs_unittest(unittest_crc src/unittest_crc.cpp src/unittest_crc_main.cpp)
cs_unittest(unittest_endian src/unittest_endian.cpp src/unittest_endian_impl.cpp src/unittest_endian_main.cpp)
cs_unittest(unittest_fileio src/unittest_fileio.cpp src/unittest_fileio_main.cpp)
cs_unittest(unittest_flags src/unittest_flags.cpp src/unittest_flags_main.cpp)
cs_unittest(unittest_format src/unittest_format.cpp src/unittest_format_main.cpp)
cs_unittest(unittest_hash src/unittest_hash.cpp src/unittest_hash_main.cpp)
cs_unittest(unittest_math src/unittest_math.cpp src/unittest_math_main.cpp)
cs_unittest(unittest_meta src/unittest_meta.cpp src/unittest_meta_main.cpp)
cs_unittest(unittest_n4 src/unittest_n4.cpp src/unittest_n4_main.cpp)
cs_unittest(unittest_numeric src/unittest_numeric.cpp src/unittest_numeric_main.cpp)
cs_unittest(unittest_pointer src/unittest_pointer.cpp src/unittest_pointer_main.cpp)
cs_unittest(unittest_serialization src/unittest_serialization.cpp src/unittest_serialization_main.cpp)
cs_unittest(unittest_simd src/unittest_simd.cpp src/unittest_simd_main.cpp)
cs_unittest(unittest_statistics src/unittest_statistics.cpp src/unittest_statistics_main.cpp)
cs_unittest(unittest_stringutil src/unittest_stringutil.cpp src/unittest_stringutil_main.cpp)
cs_unittest(unittest_text src/unittest_text.cpp src/unittest_text_main.cpp)
cs_unittest(unittest_traits src/unittest_traits.cpp src/unittest_traits_main.cpp)
cs_unittest(unittest_trie src/unittest_trie.cpp src/unittest_trie_main.cpp)

if(ENABLE_UNICODE)
  cs_unittest(unittest_textconverter src/unittest_textconverter.cpp src/unittest_textconverter_main.cpp)
endif()
