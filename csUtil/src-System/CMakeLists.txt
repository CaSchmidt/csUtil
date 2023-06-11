list(APPEND csSystem_HEADERS
  include/cs/System/FileSystem.h
  include/cs/System/Time.h
  )

list(APPEND csSystem_SOURCES
  src/FileSystem.cpp
  src/main_system.cpp
  src/Time.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csSystem_HEADERS}
  PRIVATE ${csSystem_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
