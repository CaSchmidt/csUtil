list(APPEND csQtCreator_HEADERS
  include/QtCreator/HighlightingItemDelegate.h
  )

list(APPEND csQtCreator_SOURCES
  src/HighlightingItemDelegate.cpp
  src/main_qtcreator.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csQtCreator_HEADERS}
  PRIVATE ${csQtCreator_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
