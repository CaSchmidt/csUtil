#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>
#include <numeric>

#include <catch.hpp>

#include <cs/SIMD/SIMD.h>
#include <cs/SIMD/SIMD128.h>
#include <cs/Text/PrintUtil.h>

namespace test_util {

  template<typename SIMD>
  bool equals(const typename SIMD::block_type& x,
              const typename SIMD::value_type& e0,
              const typename SIMD::value_type& e1,
              const typename SIMD::value_type& e2,
              const typename SIMD::value_type& e3)
  {
    std::array<typename SIMD::value_type,SIMD::NUM_ELEM> a;
    SIMD::store(a.data(), x);

    return a[0] == e0  &&  a[1] == e1  &&  a[2] == e2  &&  a[3] == e3;
  }

  template<typename SIMD>
  void print(const typename SIMD::block_type& x,
             const char *name = nullptr)
  {
    std::array<typename SIMD::value_type,SIMD::NUM_ELEM> a;
    SIMD::store(a.data(), x);

    if( name != nullptr ) {
      cs::print("% = ", name);
    }
    cs::print("[ % % % % ]", a[0], a[1], a[2], a[3]);
  }

} // namespace test_util

namespace test_simd {

  TEMPLATE_TEST_CASE("Various SIMD algorithms.", "[simd]", double, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using real_T = TestType;
    using simd_T = cs::simd128_t<real_T>;

    const std::array<real_T,5> values = {1, 2, 5, 8, 9};
    //                                   1  4 25 64  81
    //                                   1  5 30 94 175

    REQUIRE( cs::simd::dot<simd_T>(values.data(), values.data(), values.size()) == real_T{175} );

    REQUIRE( cs::simd::sum<simd_T>(values.data(), values.size()) == real_T{25} );

    REQUIRE( cs::simd::sum_squared<simd_T>(values.data(), values.size()) == real_T{175} );
  }

  TEMPLATE_TEST_CASE("Various SIMD algorithms (#2).", "[simd]", double, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using real_T = TestType;
    using simd_T = cs::simd128_t<real_T>;

    std::array<real_T,23> values;
    std::iota(values.begin(), values.end(), real_T{1});

    REQUIRE( cs::simd::dot<simd_T>(values.data(), values.data(), values.size()) == real_T{4324} );

    REQUIRE( cs::simd::sum<simd_T>(values.data(), values.size()) == real_T{276} );

    REQUIRE( cs::simd::sum_squared<simd_T>(values.data(), values.size()) == real_T{4324} );
  }

  TEMPLATE_TEST_CASE("SIMD Bit Operations.", "[bitops]", int32_t, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using SIMD = cs::simd128_t<TestType>;
    using block_type = typename SIMD::block_type;

    const block_type v1234 = SIMD::set(1, 2, 3, 4);
    const block_type v2222 = SIMD::set(2, 2, 2, 2);
    const block_type v9999 = SIMD::set(9, 9, 9, 9);

    {
      const block_type result = SIMD::cmov(SIMD::cmp_lt(v1234, v2222), v9999, v1234);

      test_util::print<SIMD>(v1234, "input");
      cs::print("  CMP_LT(2)  ");
      test_util::print<SIMD>(result, "result");
      cs::println("");

      REQUIRE( test_util::equals<SIMD>(result, 9, 2, 3, 4) );
    }

    {
      const block_type result = SIMD::cmov(SIMD::cmp_eq(v1234, v2222), v9999, v1234);

      test_util::print<SIMD>(v1234, "input");
      cs::print("  CMP_EQ(2)  ");
      test_util::print<SIMD>(result, "result");
      cs::println("");

      REQUIRE( test_util::equals<SIMD>(result, 1, 9, 3, 4) );
    }

    {
      const block_type result = SIMD::cmov(SIMD::cmp_gt(v1234, v2222), v9999, v1234);

      test_util::print<SIMD>(v1234, "input");
      cs::print("  CMP_GT(2)  ");
      test_util::print<SIMD>(result, "result");
      cs::println("");

      REQUIRE( test_util::equals<SIMD>(result, 1, 2, 9, 9) );
    }
  }

  TEMPLATE_TEST_CASE("SIMD Absolute Value.", "[abs]", int32_t, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using SIMD = cs::simd128_t<TestType>;
    using block_type = typename SIMD::block_type;

    const block_type v0n12n3 = SIMD::set(0, -1, 2, -3);

    {
      const block_type result = SIMD::abs(v0n12n3);

      cs::print("abs(");
      test_util::print<SIMD>(v0n12n3);
      cs::print(") -> ");
      test_util::print<SIMD>(result);
      cs::println("");

      REQUIRE( test_util::equals<SIMD>(result, 0, 1, 2, 3) );
    }
  }

  TEMPLATE_TEST_CASE("SIMD Shuffle Operations.", "[shuffle]", int32_t, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using SIMD = cs::simd128_t<TestType>;
    using block_type = typename SIMD::block_type;

    const block_type v1234 = SIMD::set(1, 2, 3, 4);
    const block_type v5678 = SIMD::set(5, 6, 7, 8);

    {
      const block_type result = SIMD::intrlvhi(v1234, v5678);

      cs::print("intrlvhi(");
      test_util::print<SIMD>(v1234);
      cs::print(", ");
      test_util::print<SIMD>(v5678);
      cs::print(") -> ");
      test_util::print<SIMD>(result);
      cs::println("");

      REQUIRE( test_util::equals<SIMD>(result, 3, 7, 4, 8) );
    }

    {
      const block_type result = SIMD::intrlvlo(v1234, v5678);

      cs::print("intrlvlo(");
      test_util::print<SIMD>(v1234);
      cs::print(", ");
      test_util::print<SIMD>(v5678);
      cs::print(") -> ");
      test_util::print<SIMD>(result);
      cs::println("");

      REQUIRE( test_util::equals<SIMD>(result, 1, 5, 2, 6) );
    }
  }

} // namespace test_simd
