macro(cs_test target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  set_target_properties(${target} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${csUtil_SOURCE_DIR}/testutils/include
  )
  target_link_libraries(${target} csUtil)
  if(ENABLE_QT)
    target_compile_definitions(${target} PRIVATE
      QT_NO_CAST_FROM_ASCII
      QT_NO_CAST_TO_ASCII
    )
    set_target_properties(${target} PROPERTIES
      AUTOMOC ON
      AUTORCC ON
      AUTOUIC ON
      AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
    )
  endif() # ENABLE_QT
  if(CMAKE_COMPILER_IS_GNUCC  OR  CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${target}
      PRIVATE -Wall
    )
  endif()
endmacro()

### Tests ####################################################################

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../testdata/test äuml.txt"
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata
)

cs_test(test_crc src/test_crc.cpp)
cs_test(test_dbhdecrypt src/test_dbhdecrypt.cpp)
cs_test(test_dot src/test_dot.cpp)
cs_test(test_endian src/test_endian.cpp)
cs_test(test_excd src/test_excd.cpp)
cs_test(test_fileio src/test_fileio.cpp)
cs_test(test_lexer src/test_lexer.cpp)
cs_test(test_property src/test_property.cpp)
cs_test(test_rawconverter src/test_rawconverter.cpp)
cs_test(test_simdbench src/test_simdbench.cpp)
cs_test(test_serial src/test_serial.cpp)
cs_test(test_stringutil src/test_stringutil.cpp)
cs_test(test_test src/test_test.cpp)
cs_test(test_trie src/test_trie.cpp)
cs_test(test_wipe src/test_wipe.cpp)

if(ENABLE_QT)
  cs_test(test_imagetip forms/imagetip.ui include/imagetip.h src/imagetip.cpp src/test_imagetip.cpp)
  cs_test(test_listeditor src/test_listeditor.cpp)
  cs_test(test_tree src/test_tree.cpp)
  cs_test(test_wprogresslogger src/test_wprogresslogger.cpp)
endif()

if(ENABLE_QT AND ENABLE_PLOT)
  cs_test(test_plot src/test_plot.cpp)
endif()

if(ENABLE_QT AND ENABLE_QTIMPORT)
  cs_test(test_codeeditor src/test_codeeditor.cpp)
  cs_test(test_highlight include/ProxyDelegate.h src/ProxyDelegate.cpp src/test_highlight.cpp)
endif()

if(ENABLE_UNICODE)
  cs_test(test_textconverter src/test_textconverter.cpp)
endif()
