add_subdirectory(calculate)
add_subdirectory(encode)
add_subdirectory(log2pcap)

if(ENABLE_QT AND ENABLE_QTIMPORT)
  add_subdirectory(BinTools)
endif()
