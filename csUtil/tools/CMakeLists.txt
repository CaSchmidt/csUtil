add_subdirectory(calculate)
add_subdirectory(encode)

if(ENABLE_QT AND ENABLE_QTIMPORT)
  add_subdirectory(BinTools)
endif()
