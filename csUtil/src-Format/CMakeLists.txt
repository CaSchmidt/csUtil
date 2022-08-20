list(APPEND csFormat_HEADERS
  include/cs/Format/Format.h
  )

list(APPEND csFormat_SOURCES
  src/Format.cpp
  src/main_format.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csFormat_HEADERS}
  PRIVATE ${csFormat_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
