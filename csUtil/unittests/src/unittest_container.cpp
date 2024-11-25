#include <cstdio>
#include <cstdlib>

#include <print>

#include <functional>
#include <iterator>
#include <list>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/Container.h>

namespace container {

  using Greater = std::greater<void>;

  using   key_type = std::string;
  using  size_type = std::size_t;
  using value_type = int;

  using KeyValue = std::pair<key_type,value_type>;
  using List     = std::list<value_type>;
  using Map      = std::unordered_map<key_type,value_type>;
  using MultiMap = std::unordered_multimap<key_type,value_type>;
  using Vector   = std::vector<value_type>;

  template<typename C>
  inline typename C::value_type get(const C& c, const std::ptrdiff_t n)
  {
    return *std::next(c.begin(), n);
  }

  template<typename C>
  inline C init()
  {
    return C{3, 1, 2};
  }

  // Key-Value Store /////////////////////////////////////////////////////////

  key_type extractor(const KeyValue& kv)
  {
    return kv.first;
  }

  void printKeyValue(const KeyValue& kv)
  {
    std::println("{}: {}", kv.first, kv.second);
  }

  void printKey(const key_type& key)
  {
    std::println("{}", key);
  }

  ////////////////////////////////////////////////////////////////////////////

  TEST_CASE("Extract elements from containers.", "[extract]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    MultiMap m;
    m.insert({"a", 11});
    m.insert({"c", 32});
    m.insert({"b", 21});
    m.insert({"a", 12});
    m.insert({"c", 31});
    m.insert({"b", 22});

    std::for_each(m.begin(), m.end(), printKeyValue);

    {
      const auto keys = cs::toSequence<std::list>(m.begin(), m.end(), extractor);

      std::println("keys (std::list<>, unsorted):");
      std::for_each(keys.begin(), keys.end(), printKey);

      REQUIRE( keys.size() == m.size() );
      REQUIRE( std::count(keys.begin(), keys.end(), "a") == std::ptrdiff_t{2} );
      REQUIRE( std::count(keys.begin(), keys.end(), "b") == std::ptrdiff_t{2} );
      REQUIRE( std::count(keys.begin(), keys.end(), "c") == std::ptrdiff_t{2} );
    }

    {
      const auto keys = cs::toSequence<std::list>(m.begin(), m.end(), extractor, true);

      std::println("keys (std::list<>, sorted):");
      std::for_each(keys.begin(), keys.end(), printKey);

      REQUIRE( keys.size() == m.size() );
      REQUIRE( get(keys, 0) == "a" );
      REQUIRE( get(keys, 1) == "a" );
      REQUIRE( get(keys, 2) == "b" );
      REQUIRE( get(keys, 3) == "b" );
      REQUIRE( get(keys, 4) == "c" );
      REQUIRE( get(keys, 5) == "c" );
    }

    {
      const auto keys = cs::toSequence<std::list>(m.begin(), m.end(), extractor, false, true);

      std::println("keys (std::list<>, unique (implies sorted)):");
      std::for_each(keys.begin(), keys.end(), printKey);

      REQUIRE( keys.size() == std::size_t{3} );
      REQUIRE( get(keys, 0) == "a" );
      REQUIRE( get(keys, 1) == "b" );
      REQUIRE( get(keys, 2) == "c" );
    }

    {
      const auto keys = cs::toSequence<std::vector>(m.begin(), m.end(), extractor, false, true);

      std::println("keys (std::vector<>, unique (implies sorted)):");
      std::for_each(keys.begin(), keys.end(), printKey);

      REQUIRE( keys.size() == std::size_t{3} );
      REQUIRE( get(keys, 0) == "a" );
      REQUIRE( get(keys, 1) == "b" );
      REQUIRE( get(keys, 2) == "c" );
    }
  } // TEST_CASE

  TEST_CASE("Reserve capacity of containers.", "[reserve]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    constexpr size_type SIZE = 127;

    {
      REQUIRE( !cs::is_reservable_v<List> );
    }

    {
      REQUIRE( cs::is_reservable_v<Vector> );

      Vector v;

      cs::reserve(v, SIZE);
      std::println("capacity = {}", v.capacity());
      REQUIRE( v.capacity() >= SIZE );
    }
  } // TEST_CASE

  TEST_CASE("Sort containers.", "[sort]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    {
      REQUIRE( cs::is_sortable_v<List> );

      List l;

      l = init<List>();
      cs::sort(l);
      REQUIRE( get(l, 0) == 1 );
      REQUIRE( get(l, 1) == 2 );
      REQUIRE( get(l, 2) == 3 );

      l = init<List>();
      cs::sort(l, Greater());
      REQUIRE( get(l, 0) == 3 );
      REQUIRE( get(l, 1) == 2 );
      REQUIRE( get(l, 2) == 1 );
    }

    {
      REQUIRE( !cs::is_sortable_v<Vector> );

      Vector v;

      v = init<Vector>();
      cs::sort(v);
      REQUIRE( get(v, 0) == 1 );
      REQUIRE( get(v, 1) == 2 );
      REQUIRE( get(v, 2) == 3 );

      v = init<Vector>();
      cs::sort(v, Greater());
      REQUIRE( get(v, 0) == 3 );
      REQUIRE( get(v, 1) == 2 );
      REQUIRE( get(v, 2) == 1 );
    }
  } // TEST_CASE

} // namespace container
