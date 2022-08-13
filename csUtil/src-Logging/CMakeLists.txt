list(APPEND csLogging_HEADERS
  include/cs/Logging/DualLogger.h
  include/cs/Logging/ILogger.h
  include/cs/Logging/IProgress.h
  include/cs/Logging/Logger.h
  include/cs/Logging/OutputContext.h
  )

list(APPEND csLogging_SOURCES
  src/DualLogger.cpp
  src/ILogger.cpp
  src/IProgress.cpp
  src/Logger.cpp
  src/main_logging.cpp
  src/OutputContext.cpp
  )

### Interface ################################################################

add_library(csLogging INTERFACE)

target_include_directories(csLogging
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csLogging
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csLogging_HEADERS}
  PRIVATE ${csLogging_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csLogging
  )
