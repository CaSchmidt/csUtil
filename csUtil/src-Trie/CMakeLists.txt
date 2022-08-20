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

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csTrie_HEADERS}
  PRIVATE ${csTrie_SOURCES}
  )

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
