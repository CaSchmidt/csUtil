#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/Bit.h>

namespace test_bit {

  TEST_CASE("Move single bit.", "[movebit]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    {
      REQUIRE( cs::moveBitR<4,1>(27) == 2 );
    }

    {
      REQUIRE( cs::moveBitR<2,0>(27) == 0 );
    }
  }

} // namespace test_bytearray
