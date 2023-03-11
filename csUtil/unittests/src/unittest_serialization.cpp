#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#include <cs/Convert/Deserialize.h>

namespace test_deserialization {

  TEST_CASE("Deserialize to unsigned integral value.", "[tounsigned]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr uint8_t VALUE{2};

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toUnsignedFromBE<uint8_t>(data.data(), 1) == VALUE );

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toUnsignedFromLE<uint8_t>(data.data(), 1) == VALUE );
    }

    { // 16bit
      constexpr uint16_t VALUE{0x0102};

      data.fill(0);
      data[0] = 1;
      data[1] = 2;
      REQUIRE( cs::toUnsignedFromBE<uint16_t>(data.data(), 2) == VALUE );
      REQUIRE( cs::toUnsignedFromBE<uint16_t>(data.data(), 1) == uint16_t{0x01} );

      data.fill(0);
      data[0] = 2;
      data[1] = 1;
      REQUIRE( cs::toUnsignedFromLE<uint16_t>(data.data(), 2) == VALUE );
      REQUIRE( cs::toUnsignedFromLE<uint16_t>(data.data(), 1) == uint16_t{0x02} );
    }

    { // 32bit
      constexpr uint32_t VALUE{0x01000002};

      data.fill(0);
      data[0] = 1;
      data[3] = 2;
      REQUIRE( cs::toUnsignedFromBE<uint32_t>(data.data(), 4) == VALUE );
      REQUIRE( cs::toUnsignedFromBE<uint32_t>(data.data(), 3) == uint32_t{0x010000} );

      data.fill(0);
      data[0] = 2;
      data[3] = 1;
      REQUIRE( cs::toUnsignedFromLE<uint32_t>(data.data(), 4) == VALUE );
      REQUIRE( cs::toUnsignedFromLE<uint32_t>(data.data(), 3) == uint32_t{0x000002} );
    }

    { // 64bit
      constexpr uint64_t VALUE{0x0100000000000002};

      data.fill(0);
      data[0] = 1;
      data[7] = 2;
      REQUIRE( cs::toUnsignedFromBE<uint64_t>(data.data(), 8) == VALUE );
      REQUIRE( cs::toUnsignedFromBE<uint64_t>(data.data(), 7) == uint64_t{0x01000000000000} );

      data.fill(0);
      data[0] = 2;
      data[7] = 1;
      REQUIRE( cs::toUnsignedFromLE<uint64_t>(data.data(), 8) == VALUE );
      REQUIRE( cs::toUnsignedFromLE<uint64_t>(data.data(), 7) == uint64_t{0x00000000000002} );
    }
  }

} // namespace test_deserialization
