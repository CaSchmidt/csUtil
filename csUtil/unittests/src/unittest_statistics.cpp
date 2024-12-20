#include <array>
#include <print>

#include <catch2/catch_template_test_macros.hpp>

#include <cs/Math/Statistics.h>

namespace test_mean {

  TEMPLATE_TEST_CASE("Compute mean of values.", "[mean]", double, float) {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    using real_T = TestType;

    const std::array<real_T,5> values = {1, 2, 5, 8, 9};

    REQUIRE( cs::mean(values.data(), values.size()) == real_T{5} );
  }

} // namespace test_mean
