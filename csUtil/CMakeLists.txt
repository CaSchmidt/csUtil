cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(csUtil)

# Custom CMake utilities
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FormatOutputName)

# Static Build?
option(BUILD_SHARED_LIBS "Build shared library" ON)

if(NOT BUILD_SHARED_LIBS)
  set(CS_BUILD_UTIL_STATIC ON)
endif()

# config.h
configure_file(include/config/csutil_config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/csUtil/csutil_config.h @ONLY
  )

# Build Tests?
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_UNITTESTS "Enable unit tests" OFF)

### 3rd Party ################################################################

add_subdirectory(3rdparty/double-conversion)
add_subdirectory(3rdparty/icu-651)

### csUtil ###################################################################

list(APPEND csUtil_HEADERS
  include/csUtil/csCharConv.h
  include/csUtil/csCharUtil.h
  include/csUtil/csEndian.h
  include/csUtil/csFileIO.h
  include/csUtil/csFlags.h
  include/csUtil/csFlatTrie.h
  include/csUtil/csFormat.h
  include/csUtil/csILogger.h
  include/csUtil/csLogger.h
  include/csUtil/csNamespace.h
  include/csUtil/csPointer.h
  include/csUtil/csRawConverter.h
  include/csUtil/csStringUtil.h
  include/csUtil/csTextConverter.h
  include/csUtil/csTrie.h
  include/csUtil/csTypeTraits.h
  include/internal/csutil_internal.h
  include/internal/csFlatTrieUtil.h
  include/internal/csTrieNode.h
  )

list(APPEND csUtil_SOURCES
  src/csCharConv-fp.cpp
  src/csCharConv.cpp
  src/csFileIO.cpp
  src/csFlatTrie.cpp
  src/csFormat.cpp
  src/csILogger.cpp
  src/csLogger.cpp
  src/csTextConverter.cpp
  src/csTrie.cpp
  )

### Target ###################################################################

add_library(csUtil
  ${csUtil_HEADERS}
  ${csUtil_SOURCES}
  )

format_output_name(csUtil "csUtil")

target_compile_definitions(csUtil
  PRIVATE CS_BUILD_UTIL_LIB
  )
target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
target_link_libraries(csUtil
  PUBLIC dblcnv
  PUBLIC icu
  )

if(WIN32)
  target_compile_options(csUtil PRIVATE /wd4251)
endif()

if(BUILD_SHARED_LIBS AND UNIX)
  target_link_libraries(csUtil dl)
endif()

install(TARGETS csUtil
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(DIRECTORY include/
  DESTINATION include
  PATTERN "config" EXCLUDE
  PATTERN "internal" EXCLUDE
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/csUtil/csutil_config.h DESTINATION include/csUtil
  )

### Tests ####################################################################

if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

if(ENABLE_UNITTESTS)
  add_subdirectory(unittests)
endif()
