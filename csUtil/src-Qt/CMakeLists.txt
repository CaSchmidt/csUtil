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

### Interface ################################################################

add_library(csQt INTERFACE)

target_include_directories(csQt
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csQt
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csQt_HEADERS}
  PRIVATE ${csQt_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csQt
  )
