#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#include <cs/Convert/Deserialize.h>

namespace test_deserialization {

  TEST_CASE("Deserialize to signed integral value.", "[signed]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr int8_t VALUE{2};

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromBE<int8_t>(data.data(), 1) == VALUE );

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromLE<int8_t>(data.data(), 1) == VALUE );
    }

    { // 16bit
      constexpr int16_t VALUE{0x0102};

      data.fill(0);
      data[0] = 1;
      data[1] = 2;
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), 2) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), 1) == int16_t{0x01} );

      data.fill(0);
      data[0] = 2;
      data[1] = 1;
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), 2) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), 1) == int16_t{0x02} );
    }

    { // 32bit
      constexpr int32_t VALUE{0x01000002};

      data.fill(0);
      data[0] = 1;
      data[3] = 2;
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), 4) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), 3) == int32_t{0x010000} );

      data.fill(0);
      data[0] = 2;
      data[3] = 1;
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), 4) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), 3) == int32_t{0x000002} );
    }

    { // 64bit
      constexpr int64_t VALUE{0x0100000000000002};

      data.fill(0);
      data[0] = 1;
      data[7] = 2;
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), 8) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), 7) == int64_t{0x01000000000000} );

      data.fill(0);
      data[0] = 2;
      data[7] = 1;
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), 8) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), 7) == int64_t{0x00000000000002} );
    }
  }

  TEST_CASE("Deserialize to signed integral value with sign extension.", "[sign_extend]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr int8_t VALUE{-2};

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int8_t>(data.data(), 1, true) == VALUE );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int8_t>(data.data(), 1, true) == VALUE );
    }

    { // 16bit
      constexpr int16_t VALUE{-2};

      data.fill(0xFF);
      data[1] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), 2, true) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), 1, true) == int16_t{-1} );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), 2, true) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), 1, true) == VALUE );
    }

    { // 32bit
      constexpr int32_t VALUE{-2};

      data.fill(0xFF);
      data[3] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), 4, true) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), 3, true) == int32_t{-1} );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), 4, true) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), 3, true) == VALUE );
    }

    { // 64bit
      constexpr int64_t VALUE{-2};

      data.fill(0xFF);
      data[7] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), 8, true) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), 7, true) == int64_t{-1} );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), 8, true) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), 7, true) == VALUE );
    }
  }

  TEST_CASE("Deserialize to unsigned integral value.", "[unsigned]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr uint8_t VALUE{0x02};

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromBE<uint8_t>(data.data(), 1) == VALUE );

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromLE<uint8_t>(data.data(), 1) == VALUE );
    }

    { // 16bit
      constexpr uint16_t VALUE{0x0102};

      data.fill(0);
      data[0] = 1;
      data[1] = 2;
      REQUIRE( cs::toIntegralFromBE<uint16_t>(data.data(), 2) == VALUE );
      REQUIRE( cs::toIntegralFromBE<uint16_t>(data.data(), 1) == uint16_t{0x01} );

      data.fill(0);
      data[0] = 2;
      data[1] = 1;
      REQUIRE( cs::toIntegralFromLE<uint16_t>(data.data(), 2) == VALUE );
      REQUIRE( cs::toIntegralFromLE<uint16_t>(data.data(), 1) == uint16_t{0x02} );
    }

    { // 32bit
      constexpr uint32_t VALUE{0x01000002};

      data.fill(0);
      data[0] = 1;
      data[3] = 2;
      REQUIRE( cs::toIntegralFromBE<uint32_t>(data.data(), 4) == VALUE );
      REQUIRE( cs::toIntegralFromBE<uint32_t>(data.data(), 3) == uint32_t{0x010000} );

      data.fill(0);
      data[0] = 2;
      data[3] = 1;
      REQUIRE( cs::toIntegralFromLE<uint32_t>(data.data(), 4) == VALUE );
      REQUIRE( cs::toIntegralFromLE<uint32_t>(data.data(), 3) == uint32_t{0x000002} );
    }

    { // 64bit
      constexpr uint64_t VALUE{0x0100000000000002};

      data.fill(0);
      data[0] = 1;
      data[7] = 2;
      REQUIRE( cs::toIntegralFromBE<uint64_t>(data.data(), 8) == VALUE );
      REQUIRE( cs::toIntegralFromBE<uint64_t>(data.data(), 7) == uint64_t{0x01000000000000} );

      data.fill(0);
      data[0] = 2;
      data[7] = 1;
      REQUIRE( cs::toIntegralFromLE<uint64_t>(data.data(), 8) == VALUE );
      REQUIRE( cs::toIntegralFromLE<uint64_t>(data.data(), 7) == uint64_t{0x00000000000002} );
    }
  }

} // namespace test_deserialization
