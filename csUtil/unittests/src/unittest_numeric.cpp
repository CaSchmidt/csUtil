#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Math/Numeric.h>

namespace numeric {

  TEST_CASE("Count digits of integral value.", "[digits]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    REQUIRE( cs::countDigits(99)  == 2 );
    REQUIRE( cs::countDigits(1)   == 1 );
    REQUIRE( cs::countDigits(0)   == 1 );
    REQUIRE( cs::countDigits(-1)  == 1 );
    REQUIRE( cs::countDigits(-99) == 2 );

    REQUIRE( cs::countDigits<int,16>(0xA)  == 1 );
    REQUIRE( cs::countDigits<int,16>(0xFF) == 2 );

    REQUIRE( cs::countDigits<int,2>(1) == 1 );
    REQUIRE( cs::countDigits<int,2>(8) == 4 );
  }

  TEST_CASE("Test parity of integral value.", "[parity]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    REQUIRE( cs::isOdd(1) );
    REQUIRE( !cs::isEven(1) );

    REQUIRE( cs::isOdd(-1) );
    REQUIRE( !cs::isEven(-1) );

    REQUIRE( cs::isEven(2) );
    REQUIRE( !cs::isOdd(2) );

    REQUIRE( cs::isEven(-2) );
    REQUIRE( !cs::isOdd(-2) );
  }

} // namespace numeric
