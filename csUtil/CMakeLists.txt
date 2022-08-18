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
  find_package(Qt5Widgets 5.12 REQUIRED)
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
  PRIVATE HAVE_STD_FORMAT
  )

if(ENABLE_QT)
  target_link_libraries(csUtilLib
    PUBLIC Qt5::Widgets
    )

  set_target_properties(csUtilLib PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    )

  # set_property(TARGET <target> APPEND PROPERTY AUTOUIC_SEARCH_PATHS <path>)
endif()

if(UNIX)
  target_link_libraries(csUtilLib
    PUBLIC pthread # std::unique_ptr<>
    )
endif()

### Modules ##################################################################

add_subdirectory(src-Core)
add_subdirectory(src-IO)
add_subdirectory(src-Logging)
add_subdirectory(src-SIMD)
add_subdirectory(src-Math) # depends on SIMD
add_subdirectory(src-System)
add_subdirectory(src-Trie)

if(ENABLE_CHARCONV)
  add_subdirectory(src-CharConv)
  add_subdirectory(src-Format) # depends on CharConv
endif()

if(ENABLE_QT)
  add_subdirectory(src-Qt)
endif()

if(ENABLE_UNICODE)
  add_subdirectory(src-Unicode)
endif()

### Interfaces ###############################################################

add_library(csUtilHeaders INTERFACE)

target_link_libraries(csUtilHeaders
  INTERFACE csIO
  INTERFACE csLogging
  INTERFACE csMath # includes SIMD
  INTERFACE csSystem
  INTERFACE csTrie
  )

if(ENABLE_CHARCONV)
  target_link_libraries(csUtilHeaders
    INTERFACE csFormat # includes CharConv
    )
endif()

if(ENABLE_UNICODE)
  target_link_libraries(csUtilHeaders
    INTERFACE csUnicode
    )
endif()

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
