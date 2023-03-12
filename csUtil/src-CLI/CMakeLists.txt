list(APPEND csCLI_HEADERS
  include/cs/CLI/Option.h
  include/cs/CLI/Options.h
)

list(APPEND csCLI_SOURCES
  src/Option.cpp
  src/Options.cpp
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
