macro(cs_unittest target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  if(MSVC)
    target_compile_options(${target}
      PRIVATE /source-charset:UTF-8 /execution-charset:UTF-8 /validate-charset
      )
  endif()
  target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${csUtil_SOURCE_DIR}/3rdparty/catch2/include
    )
  target_link_libraries(${target} csUtil)
endmacro()

### Unit Tests ###############################################################

cs_unittest(unittest_textconverter src/unittest_textconverter.cpp)