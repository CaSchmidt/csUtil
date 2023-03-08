#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#include <cs/Serialization/Deserialization.h>

namespace test_deserialization {

  TEST_CASE("Deserialize to unsigned integral value.", "[tounsigned]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr uint8_t VALUE{2};

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toUnsignedBE<uint8_t>(data.data(), data.size()) == VALUE );

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toUnsignedLE<uint8_t>(data.data(), data.size()) == VALUE );
    }

    { // 16bit
      constexpr uint16_t VALUE{0x0102};

      data.fill(0);
      data[0] = 1;
      data[1] = 2;
      REQUIRE( cs::toUnsignedBE<uint16_t>(data.data(), data.size()) == VALUE );

      data.fill(0);
      data[0] = 2;
      data[1] = 1;
      REQUIRE( cs::toUnsignedLE<uint16_t>(data.data(), data.size()) == VALUE );
    }

    { // 32bit
      constexpr uint32_t VALUE{0x01000002};

      data.fill(0);
      data[0] = 1;
      data[3] = 2;
      REQUIRE( cs::toUnsignedBE<uint32_t>(data.data(), data.size()) == VALUE );

      data.fill(0);
      data[0] = 2;
      data[3] = 1;
      REQUIRE( cs::toUnsignedLE<uint32_t>(data.data(), data.size()) == VALUE );
    }

    { // 64bit
      constexpr uint64_t VALUE{0x0100000000000002};

      data.fill(0);
      data[0] = 1;
      data[7] = 2;
      REQUIRE( cs::toUnsignedBE<uint64_t>(data.data(), data.size()) == VALUE );

      data.fill(0);
      data[0] = 2;
      data[7] = 1;
      REQUIRE( cs::toUnsignedLE<uint64_t>(data.data(), data.size()) == VALUE );
    }
  }

} // namespace test_deserialization
