list(APPEND csLogging_HEADERS
  include/cs/Logging/AbstractLogger.h
  include/cs/Logging/AbstractProgress.h
  include/cs/Logging/DualLogger.h
  include/cs/Logging/Logger.h
  include/cs/Logging/OutputContext.h
  include/cs/Logging/Progress.h
  )

list(APPEND csLogging_SOURCES
  src/AbstractLogger.cpp
  src/AbstractProgress.cpp
  src/DualLogger.cpp
  src/Logger.cpp
  src/main_logging.cpp
  src/OutputContext.cpp
  src/Progress.cpp
  )

if(ENABLE_QT)
  list(APPEND csLogging_HEADERS
    include/cs/Logging/WLogger.h
    include/cs/Logging/WProgressLogger.h
    )
  list(APPEND csLogging_SOURCES
    src/WLogger.cpp
    src/WProgressLogger.cpp
    )
endif()

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csLogging_HEADERS}
  PRIVATE ${csLogging_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
