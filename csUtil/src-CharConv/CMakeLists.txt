list(APPEND csCharConv_HEADERS
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

### Interface ################################################################

add_library(csCharConv INTERFACE)

target_include_directories(csCharConv
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csCharConv
  INTERFACE csMath
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csCharConv_HEADERS}
  PRIVATE ${csCharConv_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE dblcnv
  PRIVATE csCharConv
  )
