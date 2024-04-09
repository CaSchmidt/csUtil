#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/Bit.h>

namespace test_bit {

  TEST_CASE("Move single bit.", "[movebit]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    {
      REQUIRE( cs::moveBitR<4,1>(27) == 2 );
    }

    {
      REQUIRE( cs::moveBitR<2,0>(27) == 0 );
    }
  }

} // namespace test_bytearray
