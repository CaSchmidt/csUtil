list(APPEND csSerialization_HEADERS
  include/cs/Serialization/Deserialization.h
)

list(APPEND csSerialization_SOURCES
  src/main_serialization.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csSerialization_HEADERS}
  PRIVATE ${csSerialization_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
