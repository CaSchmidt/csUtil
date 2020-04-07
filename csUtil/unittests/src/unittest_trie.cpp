#include <csUtil/csFileIO.h>
#include <csUtil/csTrie.h>

#include <internal/csTrieNode.h>

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "util.h"

#define WORD_LIST  "../../csUtil/csUtil/testutils/data/Word-List.txt"

using StringList = std::list<std::string>;

namespace priv {

  template<typename TrieT>
  inline std::size_t findAll(const TrieT& trie, const StringList& words)
  {
    std::size_t result = 0;
    for(const StringList::value_type& word : words) {
      if( trie.find(word) == cs::ExactMatch ) {
        result++;
      }
    }
    return result;
  }

  csTrie insertAll(const StringList& words)
  {
    csTrie result;
    for(const StringList::value_type& word : words) {
      result.insert(word);
    }
    return result;
  }

  void waitKeyEnter()
  {
    printf("Press <ENTER> to continue...");
    fflush(stdin);
    fgetc(stdin);
    fflush(stdin);
  }

} // namespace priv

namespace test_compiler {

  TEST_CASE("Compiler information.", "[compiler]") {
    std::cout << "sizeof(csTrieNode): " << sizeof(csTrieNode) << std::endl;
  }

} // namespace test_compiler

namespace test_basic {

  TEST_CASE("Basic trie functionality.", "[basic]") {
    // (1) File I/O //////////////////////////////////////////////////////////

    StringList words = csReadLines(WORD_LIST, true);
    REQUIRE(words.size() > 1);

    {
      const std::string front = words.front();
      words.pop_front();
      std::cout << "Words: " << words.size() << "/" << front << std::endl;
    }

    // (2) Insertion /////////////////////////////////////////////////////////

    csTrie trie = priv::insertAll(words);

    REQUIRE(priv::findAll(trie, words) == words.size());

    // (3) Memory usage //////////////////////////////////////////////////////

    words.clear();
    // priv::waitKeyEnter();
  }

} // namespace test_basic
