list(APPEND csSIMD_HEADERS
  include/cs/SIMD/SIMD.h
  include/cs/SIMD/SIMD128.h
  include/cs/SIMD/SIMD128Util.h
  )

list(APPEND csSIMD_SOURCES
  src/main_simd.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csSIMD_HEADERS}
  PRIVATE ${csSIMD_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
