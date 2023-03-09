list(APPEND csSerialization_HEADERS
  include/cs/Serialization/ByteArrayUtil.h
  include/cs/Serialization/Deserialization.h
  include/cs/Serialization/RawConverter.h
)

list(APPEND csSerialization_SOURCES
  src/ByteArrayUtil.cpp
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
