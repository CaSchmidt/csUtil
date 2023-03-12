list(APPEND csCLI_HEADERS
)

list(APPEND csCLI_SOURCES
  src/main_cli.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csCLI_HEADERS}
  PRIVATE ${csCLI_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
