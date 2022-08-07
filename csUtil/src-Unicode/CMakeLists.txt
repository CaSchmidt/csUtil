list(APPEND csUnicode_HEADERS
  include/cs/Unicode/TextConverter.h
  )

list(APPEND csUnicode_SOURCES
  src/main_unicode.cpp
  src/TextConverter.cpp
  )

### Dependencies #############################################################

add_subdirectory(${csUtil_SOURCE_DIR}/3rdparty/icu-651
  ${csUtil_BINARY_DIR}/3rdparty/icu-651
  )

### Interface ################################################################

add_library(csUnicode INTERFACE)

target_include_directories(csUnicode
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csUnicode
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csUnicode_HEADERS}
  PRIVATE ${csUnicode_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE icu
  PRIVATE csUnicode
  )
