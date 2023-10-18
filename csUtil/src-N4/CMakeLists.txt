list(APPEND N4_HEADERS
  include/N4/BinaryOperators.h
  include/N4/Color3f.h
  include/N4/Dispatch.h
  include/N4/ExprBase.h
  include/N4/ExprCast.h
  include/N4/Functions.h
  include/N4/Matrix4f.h
  include/N4/N4.h
  include/N4/N4Traits.h
  include/N4/Normal3f.h
  include/N4/Optics.h
  include/N4/UnaryOperators.h
  include/N4/Util.h
  include/N4/Vector4f.h
  include/N4/Vertex4f.h
)

list(APPEND N4_SOURCES
  src/main_n4.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${N4_HEADERS}
  PRIVATE ${N4_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
