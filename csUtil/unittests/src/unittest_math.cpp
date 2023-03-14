#include <algorithm>
#include <iostream>

#include <catch.hpp>

#include <cs/Core/Constants.h>
#include <cs/Math/Compare.h>
#include <cs/Math/Saturate.h>

namespace test_decomposition {

  TEMPLATE_TEST_CASE("Decomposition of floating point value.", "[decompose]", double, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;
    using m = cs::math<value_type>;

    const value_type x1 = 128.75;

    REQUIRE( m::fract(x1) == 0.75 );
    REQUIRE( m::intgr(x1) == 128.0 );

    const value_type x2 = -3.5;

    REQUIRE( m::fract(x2) == -0.5 );
    REQUIRE( m::intgr(x2) == -3.0 );
  }

}

namespace test_exponentiation {

  TEMPLATE_TEST_CASE("Exponentiation of floating point value.", "[exponent]", double, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;
    using cmp = cs::cmp<value_type>;
    using   k = cs::real_konst<value_type>;
    using   m = cs::math<value_type>;

    constexpr value_type TOL = k::micro;

    REQUIRE( cmp::equal_to(m::log(k::e), 1.0, TOL) );

    REQUIRE( m::log10( 1.0) == 0.0 );
    REQUIRE( m::log10(10.0) == 1.0 );

    REQUIRE( m::pow(7.0, 0.0) == 1.0 );
    REQUIRE( m::pow(7.0, 1.0) == 7.0 );
  }

}

namespace test_rounding {

  TEMPLATE_TEST_CASE("Rounding of floating point value.", "[round]", double, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;
    using m = cs::math<value_type>;

    const value_type x1 = 1.0;
    const value_type x2 = 1.25;
    const value_type x3 = 1.5;
    const value_type x4 = 1.75;

    REQUIRE( m::ceil(x1) == 1.0 );
    REQUIRE( m::ceil(x2) == 2.0 );
    REQUIRE( m::ceil(x3) == 2.0 );
    REQUIRE( m::ceil(x4) == 2.0 );

    REQUIRE( m::floor(x1) == 1.0 );
    REQUIRE( m::floor(x2) == 1.0 );
    REQUIRE( m::floor(x3) == 1.0 );
    REQUIRE( m::floor(x4) == 1.0 );

    REQUIRE( m::round(x1) == 1.0 );
    REQUIRE( m::round(x2) == 1.0 );
    REQUIRE( m::round(x3) == 2.0 );
    REQUIRE( m::round(x4) == 2.0 );
  }

}

namespace test_saturate {

  inline int add(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a + b, min, max);
  }

  TEST_CASE("Saturated addition of integral values.", "[satadd]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    { // signed, saturated addition
      using signed_type = int8_t;

      constexpr signed_type SIGNED_MIN   = cs::konst<signed_type>::MIN;
      constexpr signed_type SIGNED_MAX   = cs::konst<signed_type>::MAX;
      constexpr std::size_t SIGNED_RANGE = cs::konst<std::size_t>::ONE << cs::NUM_BITS<signed_type>;

      signed_type a = 0;
      for(std::size_t i = 0; i < SIGNED_RANGE; i++, a++) {
        signed_type b = 0;
        for(std::size_t j = 0; j < SIGNED_RANGE; j++, b++) {
          REQUIRE( cs::saturate::add(a, b) == add(a, b, SIGNED_MIN, SIGNED_MAX) );
        } // for( b )
      } // for( a )
    }
  }

} // namespace test_saturate
