list(APPEND csCrypto_HEADERS
  include/internal/AesNi128.h
  include/cs/Crypto/BlockCipher.h
  include/cs/Crypto/CryptoUtil.h
  include/cs/Crypto/Hash.h
  include/internal/Aes.h
  include/internal/AesNi.h
)

list(APPEND csCrypto_SOURCES
  src/AesNi128.cpp
  src/BlockCipher.cpp
  src/CryptoUtil.cpp
  src/Hash.cpp
  src/main_crypto.cpp
)

### Dependencies #############################################################

add_subdirectory(${csUtil_SOURCE_DIR}/3rdparty/md5
  ${csUtil_BINARY_DIR}/3rdparty/md5
)

add_subdirectory(${csUtil_SOURCE_DIR}/3rdparty/sha
  ${csUtil_BINARY_DIR}/3rdparty/sha
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csCrypto_HEADERS}
  PRIVATE ${csCrypto_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(csUtil
  PRIVATE md5 sha
)
