list(APPEND csSystem_HEADERS
  include/cs/System/Time.h
  )

list(APPEND csSystem_SOURCES
  src/main_system.cpp
  src/Time.cpp
  )

### Interface ################################################################

add_library(csSystem INTERFACE)

target_include_directories(csSystem
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csSystem
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csSystem_HEADERS}
  PRIVATE ${csSystem_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csSystem
  )
