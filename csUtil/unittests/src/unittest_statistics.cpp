#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#define HAVE_SIMD128_PREFETCH
#include <csUtil/csStatistics.h>

namespace test_mean {

  TEMPLATE_TEST_CASE("Compute mean of values.", "[mean]", double, float) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using real_T = TestType;

    const std::array<real_T,5> values = {1, 2, 5, 8, 9};

    REQUIRE( cs::mean(values.data(), values.size()) == real_T{5} );
  }

} // namespace test_statistics
