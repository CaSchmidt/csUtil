list(APPEND csConcurrent_HEADERS
  include/cs/Concurrent/MapReduce.h
)

list(APPEND csConcurrent_SOURCES
  src/main_concurrent.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csConcurrent_HEADERS}
  PRIVATE ${csConcurrent_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
