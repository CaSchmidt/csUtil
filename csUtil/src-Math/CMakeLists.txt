list(APPEND csMath_HEADERS
  include/cs/Math/Compare.h
  include/cs/Math/Interval.h
  include/cs/Math/Math.h
  include/cs/Math/Numeric.h
  include/cs/Math/Statistics.h
  )

list(APPEND csMath_SOURCES
  src/main_math.cpp
  )

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csMath_HEADERS}
  PRIVATE ${csMath_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
