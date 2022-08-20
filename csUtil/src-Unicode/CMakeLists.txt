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

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csUnicode_HEADERS}
  PRIVATE ${csUnicode_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csUtil
  PRIVATE icu
  )
