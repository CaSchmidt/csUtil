list(APPEND csCore_HEADERS
  include/cs/Core/CharUtil.h
  include/cs/Core/Concepts.h
  include/cs/Core/Endian.h
  include/cs/Core/Flags.h
  include/cs/Core/Pointer.h
  include/cs/Core/StringUtil.h
  include/cs/Core/TypeTraits.h
  )

list(APPEND csCore_SOURCES
  src/main_core.cpp
  )

if(ENABLE_QT)
  list(APPEND csCore_HEADERS
    include/cs/Core/QStringUtil.h
    )
endif()

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csCore_HEADERS}
  PRIVATE ${csCore_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${csUtil_BINARY_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
