list(APPEND csCrypto_HEADERS
  include/cs/Crypto/BlockCipher.h
  include/cs/Crypto/CryptoUtil.h
  include/cs/Crypto/Hash.h
  include/internal/Aes.h
  include/internal/AesNi.h
  include/internal/AesNiImpl.h
)

list(APPEND csCrypto_SOURCES
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

add_subdirectory(${csUtil_SOURCE_DIR}/3rdparty/siphash
  ${csUtil_BINARY_DIR}/3rdparty/siphash
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
  PRIVATE md5 sha siphash
)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(csUtil
    PRIVATE -maes
  )
endif()
