list(APPEND csCrypto_HEADERS
  include/cs/Crypto/Hash.h
)

list(APPEND csCrypto_SOURCES
  src/Hash.cpp
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
