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

  StringList readWords()
  {
    StringList result = csReadLines(WORD_LIST, true);
    if( result.empty() ) {
      return result;
    }
    const StringList::value_type front = result.front();
    result.pop_front();
    std::cout << "File I/O: " << front << "/" << result.size() << " words" << std::endl;
    return result;
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

    const StringList words = priv::readWords();

    REQUIRE( !words.empty() );

    // (2) Insertion /////////////////////////////////////////////////////////

    const csTrie trie = priv::insertAll(words);
    std::cout << "size(trie): " << trie.size() << ", nodes: " << trie.nodeCount() << std::endl;

    REQUIRE( priv::findAll(trie, words) == words.size() );
  }

} // namespace test_basic

namespace test_flat {

  TEST_CASE("Flat trie functionality.", "[flat]") {
    // (1) File I/O //////////////////////////////////////////////////////////

    const StringList words = priv::readWords();

    REQUIRE( !words.empty() );

    // (2) Insertion /////////////////////////////////////////////////////////

    const csTrie trie = priv::insertAll(words);
    std::cout << "size(trie): " << trie.size() << ", nodes: " << trie.nodeCount() << std::endl;

    REQUIRE( priv::findAll(trie, words) == words.size() );

    // (3) Flatten ///////////////////////////////////////////////////////////

    const csFlatTrie flat = trie.flattened();
    std::cout << "size(flat): " << flat.size() << ", nodes: " << flat.nodeCount() << std::endl;

    REQUIRE( priv::findAll(flat, words) == words.size() );

    // (4) Output ////////////////////////////////////////////////////////////

    const double ratio = static_cast<double>(flat.size())/static_cast<double>(trie.size());
    std::cout << "flat vs. trie: " << ratio*100.0 << "%" << std::endl;
  }

} // namespace test_flat
