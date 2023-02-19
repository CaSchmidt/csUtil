list(APPEND csSIMD_HEADERS
  include/cs/SIMD/SIMD.h
  include/cs/SIMD/SIMD128.h
  include/cs/SIMD/SIMD128Impl.h
  include/cs/SIMD/impl/Reduce.h
  include/cs/SIMD/x86/SIMD128Integral32.h
  include/cs/SIMD/x86/SIMD128Real32.h
  include/cs/SIMD/x86/SIMD128Real64.h
  include/cs/SIMD/x86/SIMD128Util.h
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
