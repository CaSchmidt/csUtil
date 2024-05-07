list(APPEND csQtImport_HEADERS
  include/QtCreator/HighlightingItemDelegate.h
  include/QtExamples/CodeEditor.h
  )

list(APPEND csQtImport_SOURCES
  src/CodeEditor.cpp
  src/HighlightingItemDelegate.cpp
  src/main_qtimport.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csQtImport_HEADERS}
  PRIVATE ${csQtImport_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
