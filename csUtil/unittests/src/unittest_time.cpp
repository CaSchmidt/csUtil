#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <cs/System/Time.h>
#include <cs/Text/PrintUtil.h>

namespace test_time {

  using namespace std::literals::chrono_literals;

  TEST_CASE("Operations on the TimeVal class.", "[timeval]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::TimeVal(3s, 2001us).value() == 3002001 );

    REQUIRE( cs::TimeVal(4s, 3002001us).value() == 0 );
    REQUIRE( cs::TimeVal(-1s, 1us).value() == 0 );
    REQUIRE( cs::TimeVal(1s, -1us).value() == 0 );

    {
      const cs::TimeVal tv(3002001);

      REQUIRE( cs::sprint("%%", tv.secs(), tv.usecs()) == "3s2001us" );
    }

    {
      const cs::TimeVal a = cs::TimeVal(1s, 500002us);
      const cs::TimeVal b = cs::TimeVal(2s, 500003us);

      REQUIRE( (a + b).value() == 4000005 );
      REQUIRE( (a - b).value() == -1000001 );
    }
  }

} // namespace test_time
