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
option(ENABLE_PLOT "Build Plot module" OFF)
option(ENABLE_QTCREATOR "Build QtCreator module (cf. LICENSE)" OFF)
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

add_library(csUtil)

format_output_name(csUtil "csUtil")

set_target_properties(csUtil PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(csUtil
  PRIVATE CS_BUILD_UTIL_LIB
)

if(ENABLE_QT)
  target_sources(csUtil
    PRIVATE resources/csUtil.qrc
  )

  target_compile_definitions(csUtil
    PRIVATE QT_NO_CAST_FROM_ASCII
    PRIVATE QT_NO_CAST_TO_ASCII
  )

  target_link_libraries(csUtil
    PUBLIC Qt5::Widgets
  )

  set_target_properties(csUtil PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
  )

  # set_property(TARGET <target> APPEND PROPERTY AUTOUIC_SEARCH_PATHS <path>)
endif() # ENABLE_QT

if(UNIX)
  target_link_libraries(csUtil
    PUBLIC pthread # std::unique_ptr<>
  )
endif()

if(CMAKE_COMPILER_IS_GNUCC  OR  CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(csUtil
    PRIVATE -Wall
  )
endif()

### Modules ##################################################################

add_subdirectory(src-Core)
add_subdirectory(src-CLI)
add_subdirectory(src-Concurrent)
add_subdirectory(src-Convert)
add_subdirectory(src-Crypto)
add_subdirectory(src-IO)
add_subdirectory(src-Lexer)
add_subdirectory(src-Logging) # optionally depends on Qt
add_subdirectory(src-SIMD)
add_subdirectory(src-Math) # depends on SIMD
add_subdirectory(src-N4)
add_subdirectory(src-System)
add_subdirectory(src-Text)
add_subdirectory(src-Trie)

if(ENABLE_CHARCONV)
  add_subdirectory(src-CharConv)
  add_subdirectory(src-Format) # depends on CharConv
endif()

if(ENABLE_QT)
  add_subdirectory(src-Qt)
endif()

if(ENABLE_QT AND ENABLE_PLOT)
  add_subdirectory(src-Plot)
endif()

if(ENABLE_QT AND ENABLE_QTCREATOR)
  add_subdirectory(src-QtCreator)
endif()

if(ENABLE_UNICODE)
  add_subdirectory(src-Unicode)
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
