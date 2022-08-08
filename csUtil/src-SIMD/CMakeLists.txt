list(APPEND csSIMD_HEADERS
  include/cs/SIMD/SIMD.h
  include/cs/SIMD/SIMD128.h
  include/cs/SIMD/SIMD128Util.h
  )

list(APPEND csSIMD_SOURCES
  src/main_simd.cpp
  )

### Interface ################################################################

add_library(csSIMD INTERFACE)

target_include_directories(csSIMD
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csSIMD
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csSIMD_HEADERS}
  PRIVATE ${csSIMD_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csSIMD
  )
