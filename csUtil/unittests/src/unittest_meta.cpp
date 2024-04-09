#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

#include <catch2/catch_template_test_macros.hpp>

#include <cs/Core/Meta.h>

namespace test_accumulate {

  template<typename T>
  inline T fun_dot(const T sum, const std::size_t i,
                   const T *a, const T *b)
  {
    return sum + a[i]*b[i];
  }

  constexpr auto lam_dot =
      []<typename T>(const T sum, const std::size_t i,
                     const T *a, const T *b) -> T {
      return sum + a[i]*b[i];
};

  class Dot {
  public:
    template<typename T>
    inline T operator()(const T sum, const std::size_t i,
                        const T *a, const T *b) const
    {
      return sum + a[i]*b[i];
    }
  } obj_dot;

  TEMPLATE_TEST_CASE("Accumulate numeric arrays.", "[accumulate]", double, float, int) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    constexpr std::size_t N = 3;

    const value_type a[N] = {1, 2, 3};
    const value_type b[N] = {2, 3, 4};

    REQUIRE( cs::meta::accumulate<value_type,N>(0, fun_dot<value_type>, a, b) == 20 );
    REQUIRE( cs::meta::accumulate<value_type,N>(0, lam_dot, a, b) == 20 );
    REQUIRE( cs::meta::accumulate<value_type,N>(0, obj_dot, a, b) == 20 );
  }

  TEST_CASE("Concatenate strings.", "[accumulate]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using String     = std::string;
    using StringList = std::list<String>;

    constexpr auto lam_append = [](String init, const std::size_t i, const StringList& list) -> String {
      init += *std::next(list.begin(), i);
      return init;
    };

    const StringList list{ String{"123"}, String{"abc"}, String{"XYZ"} };

    REQUIRE( cs::meta::accumulate<String,3>(String{}, lam_append, list) == "123abcXYZ" );
  }

} // namespace test_accumulate

namespace test_foreach {

  template<typename T>
  inline void fun_add(const std::size_t i,
                      T *x, const T *a, const T *b)
  {
    x[i] = a[i] + b[i];
  }

  constexpr auto lam_add =
      []<typename T>(const std::size_t i,
                     T *x, const T *a, const T *b) -> void {
      x[i] = a[i] + b[i];
};

  class Add {
  public:
    template<typename T>
    inline void operator()(const std::size_t i,
                           T *x, const T *a, const T *b) const
    {
      x[i] = a[i] + b[i];
    }
  } obj_add;

  template<typename T>
  inline bool cmp(const T *a, const T *b, const std::size_t N)
  {
    for(std::size_t i = 0; i < N; i++) {
      if( a[i] != b[i] ) {
        return false;
      }
    }
    return true;
  }

  TEMPLATE_TEST_CASE("Iterate over numeric arrays.", "[for_each]", double, float, int) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    constexpr std::size_t N = 4;

    const value_type a[N] = {1, 2, 3, 4};
    const value_type b[N] = {2, 3, 4, 5};
    value_type x[N];

    const value_type res[N] = {3, 5, 7, 9};

    cs::meta::for_each<N>(fun_add<value_type>, x, a, b);
    REQUIRE( cmp(x, res, N) );

    cs::meta::for_each<N>(lam_add, x, a, b);
    REQUIRE( cmp(x, res, N) );

    cs::meta::for_each<N>(obj_add, x, a, b);
    REQUIRE( cmp(x, res, N) );
  }

} // namespace test_foreach

namespace test_algorithm {

  TEMPLATE_TEST_CASE("Standard algorithms.", "[algorithm]", int, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    constexpr value_type ONE = 1;

    constexpr std::size_t    N = 3;
    constexpr value_type VALUE = -7;

    const value_type a[N] = {1, 2, 3};
    const value_type b[N] = {2, 3, 4};
    value_type x[N];

    cs::meta::copy<N>(x, a);
    REQUIRE( std::equal(a, a + N, x) );

    constexpr value_type DOT = 20; // cf. a[], b[]
    REQUIRE( cs::meta::dot<N>(a, b, ONE) == DOT + ONE );

    cs::meta::fill<N>(x, VALUE);
    REQUIRE( std::count(x, x + N, VALUE) == N );
  }

} // namespace test_algorithm
