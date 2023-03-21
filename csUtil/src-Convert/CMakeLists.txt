list(APPEND csConvert_HEADERS
  include/cs/Convert/ByteArrayUtil.h
  include/cs/Convert/Deserialize.h
  include/cs/Convert/RawConverter.h
  include/cs/Convert/Serialize.h
)

list(APPEND csConvert_SOURCES
  src/ByteArrayUtil.cpp
  src/main_convert.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csConvert_HEADERS}
  PRIVATE ${csConvert_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
