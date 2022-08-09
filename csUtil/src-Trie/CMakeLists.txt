list(APPEND csTrie_HEADERS
  include/cs/Trie/FlatTrie.h
  include/cs/Trie/Trie.h
  include/cs/Trie/TrieMatch.h
  include/internal/FlatTrieUtil.h
  include/internal/TrieNode.h
  )

list(APPEND csTrie_SOURCES
  src/FlatTrie.cpp
  src/main_trie.cpp
  src/Trie.cpp
  )

### Interface ################################################################

add_library(csTrie INTERFACE)

target_include_directories(csTrie
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csTrie
  INTERFACE csCore
  )

### csUtil ###################################################################

target_sources(csUtilLib
  PRIVATE ${csTrie_HEADERS}
  PRIVATE ${csTrie_SOURCES}
  )

target_link_libraries(csUtilLib
  PRIVATE csTrie
  )
