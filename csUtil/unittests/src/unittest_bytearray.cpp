#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <cs/Convert/BufferUtil.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>

namespace test_bytearray {

  TEST_CASE("Conversion from/to hexadecimal string.", "[hexstring]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    { // Case 1 - Even Length
      const std::string  s1 = cs::sprint("%", cs::hexf(0xabcd));
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, true);

      REQUIRE( s1 == s2 );
    }

    { // Case 2 - Odd Length
      const std::string  s1 = cs::sprint("%", cs::hexf(0xabcde));
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, true);

      REQUIRE( "0" + s1 == s2 );
    }

    { // Case 3 - Even Length with Fill
      const std::string  s1 = cs::sprint("%", cs::hexf(0xabcd));
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, '-');

      REQUIRE( "ab-cd" == s2 );
    }

    { // Case 4 - Odd Length with Fill
      const std::string  s1 = cs::sprint("%", cs::hexf(0xabcde));
      const cs::Buffer    a = cs::toBuffer(s1);
      const std::string  s2 = cs::toString(a, '-');

      REQUIRE( "0a-bc-de" == s2 );
    }
  }

} // namespace test_bytearray
