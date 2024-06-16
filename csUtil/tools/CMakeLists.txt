add_subdirectory(calculate)
add_subdirectory(encode)
add_subdirectory(encodecli)

if(ENABLE_QT AND ENABLE_QTIMPORT)
  add_subdirectory(BinTools)
endif()
