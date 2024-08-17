### Dependencies #############################################################

add_subdirectory(${csUtil_SOURCE_DIR}/3rdparty/Catch2-3.5.3
  ${csUtil_BINARY_DIR}/3rdparty/Catch2-3.5.3
)

format_output_name(Catch2 "Catch2")
format_output_name(Catch2WithMain "Catch2WithMain")

set_target_properties(Catch2 Catch2WithMain
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  DEBUG_POSTFIX ""
)

### Macros ###################################################################

macro(cs_unittest target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  set_target_properties(${target} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  if(MSVC)
    target_compile_options(${target}
      PRIVATE /source-charset:UTF-8 /execution-charset:UTF-8 /validate-charset
    )
  endif()
  target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(${target} csUtil Catch2WithMain)
  if(CMAKE_COMPILER_IS_GNUCC  OR  CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${target}
      PRIVATE -Wall
    )
  endif()
  add_test(NAME ${target} COMMAND ${target})
endmacro()

### Unit Tests ###############################################################

enable_testing()

file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/AES/ECBVarKey128.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/AES/ECBVarKey192.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/AES/ECBVarKey256.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/AES/ECBVarTxt128.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/AES/ECBVarTxt192.rsp
  ${CMAKE_CURRENT_SOURCE_DIR}/../testdata/CAVP/AES/ECBVarTxt256.rsp
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata
)

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

cs_unittest(unittest_aes src/unittest_aes.cpp)
cs_unittest(unittest_bit src/unittest_bit.cpp)
cs_unittest(unittest_bufferedreader src/unittest_bufferedreader.cpp)
cs_unittest(unittest_bytearray src/unittest_bytearray.cpp)
cs_unittest(unittest_concurrent src/unittest_concurrent.cpp)
cs_unittest(unittest_container src/unittest_container.cpp)
cs_unittest(unittest_crc src/unittest_crc.cpp)
cs_unittest(unittest_db src/unittest_db.cpp)
cs_unittest(unittest_endian src/unittest_endian.cpp src/unittest_endian_impl.cpp)
cs_unittest(unittest_fileio src/unittest_fileio.cpp)
cs_unittest(unittest_flags src/unittest_flags.cpp)
cs_unittest(unittest_format src/unittest_format.cpp)
cs_unittest(unittest_hash src/unittest_hash.cpp)
cs_unittest(unittest_lexer src/unittest_lexer.cpp)
cs_unittest(unittest_math src/unittest_math.cpp)
cs_unittest(unittest_meta src/unittest_meta.cpp)
cs_unittest(unittest_n4 src/unittest_n4.cpp)
cs_unittest(unittest_numeric src/unittest_numeric.cpp)
cs_unittest(unittest_pointer src/unittest_pointer.cpp)
cs_unittest(unittest_serialization src/unittest_serialization.cpp)
cs_unittest(unittest_simd src/unittest_simd.cpp)
cs_unittest(unittest_statistics src/unittest_statistics.cpp)
cs_unittest(unittest_stringutil src/unittest_stringutil.cpp)
cs_unittest(unittest_stringvalue src/unittest_stringvalue.cpp)
cs_unittest(unittest_text src/unittest_text.cpp)
cs_unittest(unittest_time src/unittest_time.cpp)
cs_unittest(unittest_traits src/unittest_traits.cpp)
cs_unittest(unittest_trie src/unittest_trie.cpp)

if(ENABLE_UNICODE)
  cs_unittest(unittest_textconverter src/unittest_textconverter.cpp)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(unittest_aes
    PRIVATE -maes
  )
endif()
