list(APPEND csMath_HEADERS
  include/cs/Math/Math.h
  include/cs/Math/Numeric.h
  include/cs/Math/RawConverter.h
  )

list(APPEND csMath_SOURCES
  src/main_math.cpp
  )

### Interface ################################################################

add_library(csMath INTERFACE)

target_include_directories(csMath
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csMath
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csMath_HEADERS}
  PRIVATE ${csMath_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csMath
  )
