#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#include <csUtil/csSIMD.h>
#include <csUtil/csSIMD128.h>

namespace test_mean {

  TEMPLATE_TEST_CASE("Various SIMD algorithms.", "[simd]", double) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using real_T = TestType;
    using simd_T = cs::simd128_type<real_T>;

    const std::array<real_T,5> values = {1, 2, 5, 8, 9};
    //                                   1  4 25 64  81
    //                                   1  5 30 94 175

    REQUIRE( cs::simd::dot<simd_T>(values.data(), values.data(), values.size()) == real_T{175} );

    REQUIRE( cs::simd::sum<simd_T>(values.data(), values.size()) == real_T{25} );

    REQUIRE( cs::simd::sum_squared<simd_T>(values.data(), values.size()) == real_T{175} );
  }

} // namespace test_statistics
