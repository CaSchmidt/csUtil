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

  template<typename T, typename TestFunc, typename RefFunc>
  inline void test_run(TestFunc test_func, RefFunc ref_func)
  {
    constexpr T             MIN = cs::konst<T>::MIN;
    constexpr T             MAX = cs::konst<T>::MAX;
    constexpr std::size_t RANGE = cs::konst<std::size_t>::ONE << cs::NUM_BITS<T>;

    T a = 0;
    for(std::size_t i = 0; i < RANGE; i++, a++) {
      T b = 0;
      for(std::size_t j = 0; j < RANGE; j++, b++) {
        REQUIRE( test_func(a, b) == ref_func(a, b, MIN, MAX) );
      } // for( b )
    } // for( a )
  }

  ////// Add /////////////////////////////////////////////////////////////////

  constexpr int add(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a + b, min, max);
  }

  TEST_CASE("Saturated addition of integral values.", "[satadd]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    test_run<int8_t>(cs::saturate::add<int8_t>, add);
    test_run<int8_t>(cs::saturate::add_bl<int8_t>, add);
    test_run<uint8_t>(cs::saturate::add<uint8_t>, add);
    test_run<uint8_t>(cs::saturate::add_bl<uint8_t>, add);
  }

  ////// Mul /////////////////////////////////////////////////////////////////

  constexpr int mul(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a*b, min, max);
  }

  TEST_CASE("Saturated multiplication of integral values.", "[satmul]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    test_run<int8_t>(cs::saturate::mul<int8_t>, mul);
    test_run<uint8_t>(cs::saturate::mul<uint8_t>, mul);
  }

  ////// Sub /////////////////////////////////////////////////////////////////

  constexpr int sub(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a - b, min, max);
  }

  TEST_CASE("Saturated subtraction of integral values.", "[satsub]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    test_run<int8_t>(cs::saturate::sub<int8_t>, sub);
    test_run<int8_t>(cs::saturate::sub_bl<int8_t>, sub);
    test_run<uint8_t>(cs::saturate::sub<uint8_t>, sub);
    test_run<uint8_t>(cs::saturate::sub_bl<uint8_t>, sub);
  }

} // namespace test_saturate
