list(APPEND csCLI_HEADERS
  include/cs/CLI/BooleanOption.h
  include/cs/CLI/IntegralOption.h
  include/cs/CLI/IOption.h
  include/cs/CLI/Options.h
  include/cs/CLI/StringOption.h
)

list(APPEND csCLI_SOURCES
  src/BooleanOption.cpp
  src/IOption.cpp
  src/main_cli.cpp
  src/Options.cpp
  src/StringOption.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csCLI_HEADERS}
  PRIVATE ${csCLI_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
