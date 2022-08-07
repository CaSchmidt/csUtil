list(APPEND csFormat_HEADERS
  include/cs/Format/Format.h
  )

list(APPEND csFormat_SOURCES
  src/Format.cpp
  src/main_format.cpp
  )

### Interface ################################################################

add_library(csFormat INTERFACE)

target_include_directories(csFormat
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csFormat
  INTERFACE csCharConv
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csFormat_HEADERS}
  PRIVATE ${csFormat_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csFormat
  )
