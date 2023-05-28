list(APPEND csText_HEADERS
  include/cs/Text/Print.h
  include/cs/Text/PrintFormat.h
  include/cs/Text/PrintUtil.h
  include/cs/Text/StringAlgorithm.h
  include/cs/Text/StringUtil.h
)

list(APPEND csText_SOURCES
  src/main_text.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csText_HEADERS}
  PRIVATE ${csText_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
