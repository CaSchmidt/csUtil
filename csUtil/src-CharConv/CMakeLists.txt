list(APPEND csCharConv_HEADERS
  include/config/charconv_config.h
  include/cs/CharConv/CharConv.h
  )

list(APPEND csCharConv_SOURCES
  src/CharConv-fp.cpp
  src/CharConv.cpp
  src/main_charconv.cpp
  )

### Dependencies #############################################################

add_subdirectory(${csUtil_SOURCE_DIR}/3rdparty/double-conversion
  ${csUtil_BINARY_DIR}/3rdparty/double-conversion
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csCharConv_HEADERS}
  PRIVATE ${csCharConv_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csUtil
  PRIVATE dblcnv
  )
