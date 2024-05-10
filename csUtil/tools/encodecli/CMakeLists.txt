### Project ##################################################################

list(APPEND encodecli_SOURCES
  src/main.cpp
)

### Target ###################################################################

add_executable(encodecli)

format_output_name(encodecli "encodecli")

target_include_directories(encodecli
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(encodecli PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_sources(encodecli
  PRIVATE ${encodecli_SOURCES}
)

target_link_libraries(encodecli
  PRIVATE encode
)
