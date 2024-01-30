list(APPEND siphash_FILES
  src/siphash.c
  src/siphash.h
)

add_library(siphash STATIC
  ${siphash_FILES}
)

format_output_name(siphash "siphash")

target_include_directories(siphash
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(BUILD_SHARED_LIBS AND UNIX)
  set_target_properties(siphash PROPERTIES
    POSITION_INDEPENDENT_CODE ON
  )
endif()
