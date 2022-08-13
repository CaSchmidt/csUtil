cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src-Core/include/config/csutil_config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/cs/Core/csutil_config.h @ONLY
  )

# Build Modules?
option(ENABLE_CHARCONV "Build elementary string conversions module" OFF)
option(ENABLE_UNICODE "Build Unicode module" OFF)

# Build Tests?
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_UNITTESTS "Enable unit tests" OFF)

# Qt support
option(ENABLE_QT "Enable support for Qt" OFF)

if(ENABLE_QT)
  find_package(Qt5Widgets 5.6 REQUIRED)
endif()

### csUtil ###################################################################

list(APPEND csUtil_HEADERS
  include/internal/csutil_internal.h
  )

list(APPEND csUtil_SOURCES
  )

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

add_library(csUtilLib)

format_output_name(csUtilLib "csUtil")

set_target_properties(csUtilLib PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

target_compile_definitions(csUtilLib
  PRIVATE CS_BUILD_UTIL_LIB
  )

add_subdirectory(src-Core)
add_subdirectory(src-IO)
add_subdirectory(src-Logging)
add_subdirectory(src-SIMD)
add_subdirectory(src-Math) # depends on SIMD
add_subdirectory(src-System)
add_subdirectory(src-Trie)

if(ENABLE_UNICODE)
  add_subdirectory(src-Unicode)
endif()

if(ENABLE_CHARCONV)
  add_subdirectory(src-CharConv)
  add_subdirectory(src-Format) # depends on CharConv
endif()

#add_library(csUtil
#  ${csUtil_HEADERS}
#  ${csUtil_SOURCES}
#  )

#if(ENABLE_QT)
#  target_sources(csUtil
#    PRIVATE ${csUtil_FORMS}
#    )
#endif()

#set_cxx_properties(csUtil)

#set_target_properties(csUtil PROPERTIES
#  CXX_STANDARD 20
#  CXX_STANDARD_REQUIRED ON
#  )

#if(ENABLE_QT)
#  set_target_properties(csUtil PROPERTIES
#    AUTOMOC ON
#    AUTOUIC ON
#    AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
#    )
#endif()

#target_compile_definitions(csUtil
#  PRIVATE CS_BUILD_UTIL_LIB
#  PRIVATE HAVE_STD_FORMAT
#  )

#if(MSVC)
#  target_compile_options(csUtil
#    PRIVATE /wd4251
#    )
#endif()

#target_include_directories(csUtil
#  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
#  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
#  )

#list(APPEND csUtil_LIBS
#  PUBLIC dblcnv
#  PUBLIC icu
#  )
#if(ENABLE_QT)
#  list(APPEND csUtil_LIBS
#    PUBLIC Qt5::Widgets
#    )
#endif()
#if(UNIX)
#  list(APPEND csUtil_LIBS
#    PUBLIC pthread # std::unique_ptr<>
#    )
#endif()
#target_link_libraries(csUtil
#  ${csUtil_LIBS}
#  )

### Install ##################################################################

#install(TARGETS csUtil
#  ARCHIVE DESTINATION lib
#  LIBRARY DESTINATION lib
#  RUNTIME DESTINATION bin
#  )

#install(DIRECTORY include/
#  DESTINATION include
#  PATTERN "config" EXCLUDE
#  PATTERN "internal" EXCLUDE
#  )

#install(FILES
#  ${CMAKE_CURRENT_BINARY_DIR}/include/csUtil/csutil_config.h DESTINATION include/csUtil
#  )

### Tests ####################################################################

if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

if(ENABLE_UNITTESTS)
  add_subdirectory(unittests)
endif()
