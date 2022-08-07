list(APPEND csCore_HEADERS
  include/cs/Core/CharUtil.h
  include/cs/Core/Concepts.h
  include/cs/Core/Endian.h
  include/cs/Core/Flags.h
  include/cs/Core/Pointer.h
  include/cs/Core/RawConverter.h
  include/cs/Core/StringUtil.h
  include/cs/Core/TypeTraits.h
  )

list(APPEND csCore_SOURCES
  src/main_core.cpp
  )

### Interface ################################################################

add_library(csCore INTERFACE)

target_include_directories(csCore
  INTERFACE ${csUtil_BINARY_DIR}/include
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csCore_HEADERS}
  PRIVATE ${csCore_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csCore
  )
