#include <algorithm>
#include <print>

#include <catch2/catch_template_test_macros.hpp>

#include <cs/Core/Constants.h>
#include <cs/Math/Compare.h>
#include <cs/Math/Interval.h>
#include <cs/Math/Saturate.h>

namespace test_decomposition {

  TEMPLATE_TEST_CASE("Decomposition of floating point value.", "[decompose]", double, float) {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    using value_type = TestType;
    using m = cs::Math<value_type>;

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
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    using value_type = TestType;
    using cmp = cs::Compare<value_type>;
    using   k = cs::RealKonst<value_type>;
    using   m = cs::Math<value_type>;

    constexpr value_type TOL = k::micro;

    REQUIRE( cmp::equal_to(m::log(k::e), 1.0, TOL) );

    REQUIRE( m::log10( 1.0) == 0.0 );
    REQUIRE( m::log10(10.0) == 1.0 );

    REQUIRE( m::pow(7.0, 0.0) == 1.0 );
    REQUIRE( m::pow(7.0, 1.0) == 7.0 );
  }

}

namespace test_interval {

  TEMPLATE_TEST_CASE("Interval operations & queries.", "[interval]", double, float) {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    using value_type = TestType;
    using   Interval = cs::Interval<value_type>;

    const Interval intv12(1, 2);
    const Interval intv23(2, 3);
    const Interval intv34(3, 4);

    {
      constexpr value_type C = 1.5;
      constexpr value_type R = 0.5;

      REQUIRE( intv12.center() == C );
      REQUIRE( intv12.radius() == R );
    }

    { // Intersection
      constexpr value_type TOL = 0.5;

      const bool isIsect1223 = intv12  &&  intv23;
      REQUIRE( isIsect1223 );

      const bool isIsect1234 = intv12  &&  intv34;
      REQUIRE( !isIsect1234 );

      REQUIRE(  cs::intersects(intv12, Interval(1.5, 3), TOL) );
      REQUIRE( !cs::intersects(intv12, Interval(1.75, 3), TOL) );
      REQUIRE( !cs::intersects(intv12, intv23, TOL) );

      REQUIRE( !cs::intersects(intv12, Interval()) );
    }

    { // Union
      constexpr value_type TWO = 2;

      const Interval u1223 = intv12 | intv23;
      REQUIRE( u1223.range() == TWO );

      const Interval u1234 = intv12 | intv34;
      REQUIRE( !u1234.isValid() );

      const Interval u122300 = u1223 | Interval();
      REQUIRE( !u122300.isValid() );
    }

    { // Bounding Interval
      const value_type THREE = 3;

      const Interval b1234 = intv12 ^ intv34;
      REQUIRE( b1234.range() == THREE );

      const Interval b123400 = b1234 ^ Interval();
      REQUIRE( !b123400.isValid() );
    }
  }

} // namespace test_interval

namespace test_rounding {

  TEMPLATE_TEST_CASE("Rounding of floating point value.", "[round]", double, float) {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    using value_type = TestType;
    using m = cs::Math<value_type>;

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
    constexpr T             MIN = cs::Konst<T>::MIN;
    constexpr T             MAX = cs::Konst<T>::MAX;
    constexpr std::size_t RANGE = cs::Konst<std::size_t>::ONE << cs::NUM_BITS<T>;

    T a = 0;
    for(std::size_t i = 0; i < RANGE; i++, a++) {
      T b = 0;
      for(std::size_t j = 0; j < RANGE; j++, b++) {
        REQUIRE( test_func(a, b) == ref_func(a, b, MIN, MAX) );
      } // for( b )
    } // for( a )
  }

  template<typename T, typename TestFunc, typename RefFunc>
  inline void test_run_1D(TestFunc test_func, RefFunc ref_func)
  {
    constexpr T             MIN = cs::Konst<T>::MIN;
    constexpr T             MAX = cs::Konst<T>::MAX;
    constexpr std::size_t RANGE = cs::Konst<std::size_t>::ONE << cs::NUM_BITS<T>;

    T x = 0;
    for(std::size_t i = 0; i < RANGE; i++) {
      REQUIRE( test_func(x) == ref_func(x, MIN, MAX) );
    }
  }

  ////// Add /////////////////////////////////////////////////////////////////

  constexpr int add(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a + b, min, max);
  }

  constexpr int inc(const int x, const int min, const int max)
  {
    return std::clamp<int>(x + 1, min, max);
  }

  TEST_CASE("Saturated addition of integral values.", "[satadd]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    test_run<int8_t>(cs::saturate::add<int8_t>, add);
    test_run<int8_t>(cs::saturate::add_bl<int8_t>, add);
    test_run<uint8_t>(cs::saturate::add<uint8_t>, add);
    test_run<uint8_t>(cs::saturate::add_bl<uint8_t>, add);

    test_run_1D<int8_t>(cs::saturate::inc<int8_t>, inc);
    test_run_1D<int8_t>(cs::saturate::inc_bl<int8_t>, inc);
    test_run_1D<uint8_t>(cs::saturate::inc<uint8_t>, inc);
    test_run_1D<uint8_t>(cs::saturate::inc_bl<uint8_t>, inc);
  }

  ////// Mul /////////////////////////////////////////////////////////////////

  constexpr int mul(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a*b, min, max);
  }

  TEST_CASE("Saturated multiplication of integral values.", "[satmul]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    test_run<int8_t>(cs::saturate::mul<int8_t>, mul);
    test_run<uint8_t>(cs::saturate::mul<uint8_t>, mul);
  }

  ////// Sub /////////////////////////////////////////////////////////////////

  constexpr int dec(const int x, const int min, const int max)
  {
    return std::clamp<int>(x - 1, min, max);
  }

  constexpr int sub(const int a, const int b, const int min, const int max)
  {
    return std::clamp<int>(a - b, min, max);
  }

  TEST_CASE("Saturated subtraction of integral values.", "[satsub]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    test_run_1D<int8_t>(cs::saturate::dec<int8_t>, dec);
    test_run_1D<int8_t>(cs::saturate::dec_bl<int8_t>, dec);
    test_run_1D<uint8_t>(cs::saturate::dec<uint8_t>, dec);
    test_run_1D<uint8_t>(cs::saturate::dec_bl<uint8_t>, dec);

    test_run<int8_t>(cs::saturate::sub<int8_t>, sub);
    test_run<int8_t>(cs::saturate::sub_bl<int8_t>, sub);
    test_run<uint8_t>(cs::saturate::sub<uint8_t>, sub);
    test_run<uint8_t>(cs::saturate::sub_bl<uint8_t>, sub);
  }

} // namespace test_saturate
