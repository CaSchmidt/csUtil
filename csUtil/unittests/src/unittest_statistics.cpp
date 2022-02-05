#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#include <csUtil/csStatistics.h>

using real_T = double;

namespace test_mean {

  TEST_CASE("Compute mean of values.", "[mean]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const std::array<real_T,4> values = {1, 2, 5, 8};

    REQUIRE( cs::mean(values.data(), values.size()) == real_T{4} );
  }

} // namespace test_statistics
