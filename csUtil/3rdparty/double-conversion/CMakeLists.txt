list(APPEND dblcnv_FILES
  src/bignum-dtoa.cc
  src/bignum-dtoa.h
  src/bignum.cc
  src/bignum.h
  src/cached-powers.cc
  src/cached-powers.h
  src/diy-fp.cc
  src/diy-fp.h
  src/double-conversion.cc
  src/double-conversion.h
  src/fast-dtoa.cc
  src/fast-dtoa.h
  src/fixed-dtoa.cc
  src/fixed-dtoa.h
  src/ieee.h
  src/strtod.cc
  src/strtod.h
  src/utils.h
  )

add_library(dblcnv STATIC
  ${dblcnv_FILES}
  )

format_output_name(dblcnv "dblcnv")

target_include_directories(dblcnv
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

if(BUILD_SHARED_LIBS AND UNIX)
  set_target_properties(dblcnv PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
