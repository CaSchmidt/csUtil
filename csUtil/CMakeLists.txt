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

# Qt support
option(ENABLE_QT "Enable support for Qt" OFF)

if(ENABLE_QT)
  find_package(Qt5Widgets 5.6 REQUIRED)
endif()

### 3rd Party ################################################################

add_subdirectory(3rdparty/double-conversion)
add_subdirectory(3rdparty/icu-651)

### csUtil ###################################################################

list(APPEND csUtil_HEADERS
  include/csUtil/csCharConv.h
  include/csUtil/csCharUtil.h
  include/csUtil/csDualLogger.h
  include/csUtil/csEndian.h
  include/csUtil/csFile.h
  include/csUtil/csFileIO.h
  include/csUtil/csFlags.h
  include/csUtil/csFlatTrie.h
  include/csUtil/csFormat.h
  include/csUtil/csILogger.h
  include/csUtil/csIODevice.h
  include/csUtil/csIProgress.h
  include/csUtil/csLogger.h
  include/csUtil/csMath.h
  include/csUtil/csNamespace.h
  include/csUtil/csOutputContext.h
  include/csUtil/csPointer.h
  include/csUtil/csRawConverter.h
  include/csUtil/csSerial.h
  include/csUtil/csStatistics.h
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
  src/csDualLogger.cpp
  src/csFile_generic.cpp
  src/csFileIO.cpp
  src/csFlatTrie.cpp
  src/csFormat.cpp
  src/csILogger.cpp
  src/csIODevice.cpp
  src/csIProgress.cpp
  src/csLogger.cpp
  src/csOutputContext.cpp
  src/csTextConverter.cpp
  src/csTrie.cpp
  )

if(WIN32)
  list(APPEND csUtil_HEADERS
    include/internal/Win32Handle.h
    )
  list(APPEND csUtil_SOURCES
    src/csFile_win32.cpp
    src/csSerial_win32.cpp
    src/Win32Handle.cpp
    )
endif()

if(ENABLE_QT)
  list(APPEND csUtil_FORMS
    forms/csWProgressLogger.ui
    )
  list(APPEND csUtil_HEADERS
    include/csUtil/csQStringUtil.h
    include/csUtil/csWLogger.h
    include/csUtil/csWProgressLogger.h
    )
  list(APPEND csUtil_SOURCES
    src/csWLogger.cpp
    src/csWProgressLogger.cpp
    )
endif()

### Target ###################################################################

add_library(csUtil
  ${csUtil_HEADERS}
  ${csUtil_SOURCES}
  )

if(ENABLE_QT)
  target_sources(csUtil
    PRIVATE ${csUtil_FORMS}
    )
endif()

format_output_name(csUtil "csUtil")

set_target_properties(csUtil PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

if(ENABLE_QT)
  set_target_properties(csUtil PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
    )
endif()

target_compile_definitions(csUtil
  PRIVATE CS_BUILD_UTIL_LIB
  )

if(MSVC)
  target_compile_options(csUtil
    PRIVATE /wd4251
    )
endif()

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

list(APPEND csUtil_LIBS
  PUBLIC dblcnv
  PUBLIC icu
  )
if(ENABLE_QT)
  list(APPEND csUtil_LIBS
    PUBLIC Qt5::Widgets
    )
endif()
if(UNIX)
  list(APPEND csUtil_LIBS
    PUBLIC pthread # std::unique_ptr<>
    )
endif()
target_link_libraries(csUtil
  ${csUtil_LIBS}
  )

### Install ##################################################################

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
