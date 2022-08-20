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

if(ENABLE_QT)
  list(APPEND csLogging_FORMS
    forms/WProgressLogger.ui
    )
  list(APPEND csLogging_HEADERS
    include/cs/Logging/WLogger.h
    include/cs/Logging/WProgressLogger.h
    )
  list(APPEND csLogging_SOURCES
    src/WLogger.cpp
    src/WProgressLogger.cpp
    )
endif()

### Interface ################################################################

add_library(csLogging INTERFACE)

target_include_directories(csLogging
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

if(ENABLE_QT)
  target_link_libraries(csLogging
    INTERFACE csQt
    )
else()
  target_link_libraries(csLogging
    INTERFACE csCore
    )
endif()

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csLogging_HEADERS}
  PRIVATE ${csLogging_SOURCES}
  )

if(ENABLE_QT)
  target_sources(csUtilLib
    PRIVATE ${csLogging_FORMS}
    )

  set_property(TARGET csUtilLib
    APPEND PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
    )
endif()

target_link_libraries(csUtilLib
  PRIVATE csLogging
  )
