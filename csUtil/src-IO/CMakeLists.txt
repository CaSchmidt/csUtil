list(APPEND csIO_HEADERS
  include/cs/IO/File.h
  include/cs/IO/FileIO.h
  include/cs/IO/IODevice.h
  include/cs/IO/Serial.h
  )

list(APPEND csIO_SOURCES
  src/File_generic.cpp
  src/FileIO.cpp
  src/IODevice.cpp
  src/main_io.cpp
  )

if(WIN32)
  list(APPEND csIO_HEADERS
    include/internal/Win32Handle.h
    )
  list(APPEND csIO_SOURCES
    src/File_win32.cpp
    src/Serial_win32.cpp
    src/Win32Handle.cpp
    )
endif()

### Interfaces ###############################################################

add_library(csIO INTERFACE)

target_include_directories(csIO
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csIO
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csIO_HEADERS}
  PRIVATE ${csIO_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csIO
  )
