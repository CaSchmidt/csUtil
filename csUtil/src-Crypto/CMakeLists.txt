list(APPEND csCrypto_HEADERS
)

list(APPEND csCrypto_SOURCES
  src/main_crypto.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csCrypto_HEADERS}
  PRIVATE ${csCrypto_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
