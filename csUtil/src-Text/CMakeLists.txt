list(APPEND csText_HEADERS
  include/cs/Text/KeyValue.h
  include/cs/Text/MixIn.h
  include/cs/Text/Print.h
  include/cs/Text/PrintFormat.h
  include/cs/Text/PrintUtil.h
  include/cs/Text/StringAlgorithm.h
  include/cs/Text/StringClassifyImpl.h
  include/cs/Text/StringQueryImpl.h
  include/cs/Text/StringReplaceImpl.h
  include/cs/Text/StringSplitImpl.h
  include/cs/Text/StringUtil.h
  include/cs/Text/StringUtilImpl.h
  include/cs/Text/StringValue.h
  include/cs/Text/TextIO.h
)

list(APPEND csText_SOURCES
  src/main_text.cpp
  src/TextIO.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csText_HEADERS}
  PRIVATE ${csText_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
