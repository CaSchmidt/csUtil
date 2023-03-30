list(APPEND sha_FILES
  src/sha.h
  src/sha-private.h
  src/sha1.c
  src/sha224-256.c
  src/sha384-512.c
)

add_library(sha STATIC
  ${sha_FILES}
)

format_output_name(sha "sha")

target_compile_definitions(sha
  PRIVATE USE_MODIFIED_MACROS
)

target_include_directories(sha
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(BUILD_SHARED_LIBS AND UNIX)
  set_target_properties(sha PROPERTIES
    POSITION_INDEPENDENT_CODE ON
  )
endif()
