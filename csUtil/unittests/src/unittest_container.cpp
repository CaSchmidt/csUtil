#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <functional>
#include <iterator>
#include <list>
#include <vector>

#include <catch.hpp>

#include <cs/Core/Container.h>

namespace container {

  using Greater = std::greater<void>;

  using value_type = int;

  using List   = std::list<value_type>;
  using Vector = std::vector<value_type>;

  template<typename C>
  inline value_type get(const C& c, const std::ptrdiff_t n)
  {
    return *std::next(c.cbegin(), n);
  }

  template<typename C>
  inline C init()
  {
    return C{3, 1, 2};
  }

  TEST_CASE("Sort containers.", "[sort]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

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
  }

} // namespace container
