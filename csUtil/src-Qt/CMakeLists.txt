list(APPEND csQt_HEADERS
  include/cs/Qt/DialogButtonBox.h
  include/cs/Qt/FilterCancel.h
  include/cs/Qt/Layout.h
  )

list(APPEND csQt_SOURCES
  src/DialogButtonBox.cpp
  src/FilterCancel.cpp
  src/main_qt.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csQt_HEADERS}
  PRIVATE ${csQt_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
