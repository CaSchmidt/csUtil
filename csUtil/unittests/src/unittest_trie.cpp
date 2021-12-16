#include <unordered_set>

#include <csUtil/csFileIO.h>
#include <csUtil/csTrie.h>

#include <internal/csTrieNode.h>

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "util.h"

#undef HAVE_BENCHMARK

#define WORD_LIST  u8"../../csUtil/csUtil/testutils/data/Word-List.txt"

class FlatSet {
public:
  using     String = std::string;
  using      Store = std::vector<String>;
  using  ConstIter = Store::const_iterator;
  using      Range = std::pair<ConstIter,ConstIter>;
  using StringList = std::list<String>;

  template<typename InputIt>
  FlatSet(InputIt first, InputIt last) noexcept
    : _v{first, last}
  {
    std::sort(_v.begin(), _v.end());
    const ConstIter it = std::unique(_v.begin(), _v.end());
    _v.erase(it, _v.end());
  }

  ~FlatSet() noexcept = default;

  StringList complete(const String& s) const
  {
    const Range r = range(s);
    return StringList{r.first, r.second};
  }

  cs::TrieMatch find(const String& s) const
  {
    const Range r = range(s);
    const auto diff = std::distance(r.first, r.second);
    if( diff >= 1 ) {
      if( *(r.first) == s ) {
        return cs::TrieMatch::Exact;
      } else {
        return cs::TrieMatch::Partial;
      }
    }
    return cs::TrieMatch::None;
  }

  std::size_t size() const
  {
    return std::accumulate(_v.cbegin(), _v.cend(), sizeof(Store),
                           [](const std::size_t& init, const String& elem) -> std::size_t {
      return init + sizeof(String) + sizeof(String::value_type)*elem.size();
    });
  }

private:
  Range range(const String& s) const
  {
    return std::equal_range(_v.cbegin(), _v.cend(), s,
                            [&](const String& a, const String& b) -> bool {
      return a.substr(0, s.size()) < b.substr(0, s.size());
    });
  }

  Store _v;
};

using Occurrence = std::pair<std::string,std::size_t>;

using Set = std::unordered_set<std::string>;

using StringList = std::list<std::string>;

namespace priv {

  const Occurrence occur_THE{"THE", 352};

  template<typename TrieT>
  inline bool complete(const TrieT& trie, const Occurrence& o)
  {
    return trie.complete(o.first).size() == o.second;
  }

  template<typename TrieT>
  inline bool findAll(const TrieT& trie, const StringList& words)
  {
    std::size_t count = 0;
    for(const StringList::value_type& word : words) {
      if( trie.find(word) == cs::TrieMatch::Exact ) {
        count++;
      }
    }
    return count == words.size();
  }

  inline bool findAll(const Set& set, const StringList& words)
  {
    std::size_t count = 0;
    for(const StringList::value_type& word : words) {
      if( set.count(word) == 1 ) {
        count++;
      }
    }
    return count == words.size();
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
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::cout << "sizeof(csTrieNode): " << sizeof(csTrieNode) << std::endl;

    std::cout << "sizeof(std::string): " << sizeof(std::string) << std::endl;

    std::cout << "sizeof(std::list<int>): " << sizeof(std::list<int>) << std::endl;

    std::cout << "sizeof(std::list<std::string>): " << sizeof(std::list<std::string>) << std::endl;

    std::cout << "sizeof(std::vector<int>): " << sizeof(std::vector<int>) << std::endl;

    std::cout << "sizeof(std::vector<std::string>): " << sizeof(std::vector<std::string>) << std::endl;
  }

} // namespace test_compiler

namespace test_basic {

  TEST_CASE("csTrie functionality.", "[trie]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // (1) File I/O //////////////////////////////////////////////////////////

    const StringList words = priv::readWords();

    REQUIRE( !words.empty() );

    // (2) Insertion /////////////////////////////////////////////////////////

    const csTrie trie = priv::insertAll(words);
    std::cout << "size(trie): " << trie.size() << ", nodes: " << trie.nodeCount() << std::endl;

    REQUIRE( priv::findAll(trie, words) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("findAll(trie)") {
      return priv::findAll(trie, words);
    };
#endif

    // (3) Completion ////////////////////////////////////////////////////////

    REQUIRE( priv::complete(trie, priv::occur_THE) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("complete(trie)") {
      return priv::complete(trie, priv::occur_THE);
    };
#endif
  }

} // namespace test_basic

namespace test_flat {

  TEST_CASE("csFlatTrie functionality.", "[flattrie]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // (1) File I/O //////////////////////////////////////////////////////////

    const StringList words = priv::readWords();

    REQUIRE( !words.empty() );

    // (2) Insertion /////////////////////////////////////////////////////////

    const csTrie trie = priv::insertAll(words);
    std::cout << "size(trie): " << trie.size() << ", nodes: " << trie.nodeCount() << std::endl;

    REQUIRE( priv::findAll(trie, words) );

    // (3) Flatten ///////////////////////////////////////////////////////////

    const csFlatTrie flat = trie.flattened();
    std::cout << "size(flat): " << flat.size() << ", nodes: " << flat.nodeCount() << std::endl;

    REQUIRE( priv::findAll(flat, words) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("findAll(flat)") {
      return priv::findAll(flat, words);
    };
#endif

    // (4) Completion ////////////////////////////////////////////////////////

    REQUIRE( priv::complete(flat, priv::occur_THE) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("complete(flat)") {
      return priv::complete(flat, priv::occur_THE);
    };
#endif

    // (5) Output ////////////////////////////////////////////////////////////

    const double ratio = static_cast<double>(flat.size())/static_cast<double>(trie.size());
    std::cout << "flat vs. trie: " << ratio*100.0 << "%" << std::endl;
  }

} // namespace test_flat

namespace test_flatset {

  TEST_CASE("FlatSet functionality.", "[flatset]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // (1) File I/O //////////////////////////////////////////////////////////

    const StringList words = priv::readWords();

    REQUIRE( !words.empty() );

    // (2) Insertion /////////////////////////////////////////////////////////

    const FlatSet flatset{words.cbegin(), words.cend()};

    REQUIRE( priv::findAll(flatset, words) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("findAll(flatset)") {
      return priv::findAll(flatset, words);
    };
#endif

    // (3) Completion ////////////////////////////////////////////////////////

    REQUIRE( priv::complete(flatset, priv::occur_THE) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("complete(flatset)") {
      return priv::complete(flatset, priv::occur_THE);
    };
#endif

    // (4) Output ////////////////////////////////////////////////////////////

    std::cout << "size(flatset): " << flatset.size() << std::endl;
  }

} // namespace test_flatset

namespace test_unordered_set {

  TEST_CASE("std::unordered_set<> functionality.", "[unordered_set]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // (1) File I/O //////////////////////////////////////////////////////////

    const StringList words = priv::readWords();

    REQUIRE( !words.empty() );

    // (2) Insertion /////////////////////////////////////////////////////////

    const Set set{words.cbegin(), words.cend()};

    REQUIRE( priv::findAll(set, words) );

#ifdef HAVE_BENCHMARK
    BENCHMARK("findAll(set)") {
      return priv::findAll(set, words);
    };
#endif
  }

} // namespace test_unordered_set
