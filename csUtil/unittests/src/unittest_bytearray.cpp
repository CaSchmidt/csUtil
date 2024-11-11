#include <format>
#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <cs/Convert/BufferUtil.h>

namespace test_bytearray {

  TEST_CASE("Conversion from/to hexadecimal string.", "[hexstring]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    { // Case 1 - Even Length
      const std::string  s1 = std::format("{:X}", uint16_t{0xabcd});
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, true);

      REQUIRE( s1 == s2 );
    }

    { // Case 2 - Odd Length
      const std::string  s1 = std::format("{:X}", uint32_t{0xabcde});
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, true);

      REQUIRE( "0" + s1 == s2 );
    }

    { // Case 3 - Even Length with Fill
      const std::string  s1 = std::format("{:x}", uint16_t{0xabcd});
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, '-');

      REQUIRE( "ab-cd" == s2 );
    }

    { // Case 4 - Odd Length with Fill
      const std::string  s1 = std::format("{:x}", uint32_t{0xabcde});
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, '-');

      REQUIRE( "0a-bc-de" == s2 );
    }
  }

} // namespace test_bytearray
