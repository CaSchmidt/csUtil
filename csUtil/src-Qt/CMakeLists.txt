list(APPEND csQt_HEADERS
  include/cs/Qt/Clipboard.h
  include/cs/Qt/DialogButtonBox.h
  include/cs/Qt/FilterCancel.h
  include/cs/Qt/ItemModel.h
  include/cs/Qt/Layout.h
  include/cs/Qt/Meta.h
  include/cs/Qt/Widget.h
  )

list(APPEND csQt_SOURCES
  src/Clipboard.cpp
  src/DialogButtonBox.cpp
  src/FilterCancel.cpp
  src/ItemModel.cpp
  src/main_qt.cpp
  src/Widget.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csQt_HEADERS}
  PRIVATE ${csQt_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
