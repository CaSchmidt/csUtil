list(APPEND md5_FILES
  src/md5.h
  src/md5.c
)

add_library(md5 STATIC
  ${md5_FILES}
)

format_output_name(md5 "md5")

target_include_directories(md5
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(BUILD_SHARED_LIBS AND UNIX)
  set_target_properties(md5 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
  )
endif()
