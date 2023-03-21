#include <cstdio>
#include <cstdlib>

#include <array>
#include <iostream>

#include <catch.hpp>

#include <cs/Convert/Serialize.h>
#include <cs/Convert/Deserialize.h>

namespace test_deserialize {

  TEST_CASE("Deserialize to signed integral value.", "[signed]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr int8_t      VALUE{2};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromBE<int8_t>(data.data(), RANGE) == VALUE );

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromLE<int8_t>(data.data(), RANGE) == VALUE );
    }

    { // 16bit
      constexpr int16_t     VALUE{0x0102};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 1;
      data[1] = 2;
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), RANGE - 1) == int16_t{0x01} );

      data.fill(0);
      data[0] = 2;
      data[1] = 1;
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), RANGE - 1) == int16_t{0x02} );
    }

    { // 32bit
      constexpr int32_t     VALUE{0x01000002};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 1;
      data[3] = 2;
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), RANGE - 1) == int32_t{0x010000} );

      data.fill(0);
      data[0] = 2;
      data[3] = 1;
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), RANGE - 1) == int32_t{0x000002} );
    }

    { // 64bit
      constexpr int64_t     VALUE{0x0100000000000002};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 1;
      data[7] = 2;
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), RANGE - 1) == int64_t{0x01000000000000} );

      data.fill(0);
      data[0] = 2;
      data[7] = 1;
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), RANGE - 1) == int64_t{0x00000000000002} );
    }
  }

  TEST_CASE("Deserialize to signed integral value with sign extension.", "[sign_extend]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr int8_t      VALUE{-2};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int8_t>(data.data(), RANGE, true) == VALUE );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int8_t>(data.data(), RANGE, true) == VALUE );
    }

    { // 16bit
      constexpr int16_t     VALUE{-2};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0xFF);
      data[1] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), RANGE    , true) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int16_t>(data.data(), RANGE - 1, true) == int16_t{-1} );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), RANGE    , true) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int16_t>(data.data(), RANGE - 1, true) == VALUE );
    }

    { // 32bit
      constexpr int32_t     VALUE{-2};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0xFF);
      data[3] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), RANGE    , true) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int32_t>(data.data(), RANGE - 1, true) == int32_t{-1} );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), RANGE    , true) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int32_t>(data.data(), RANGE - 1, true) == VALUE );
    }

    { // 64bit
      constexpr int64_t     VALUE{-2};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0xFF);
      data[7] = 0xFE;
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), RANGE    , true) == VALUE );
      REQUIRE( cs::toIntegralFromBE<int64_t>(data.data(), RANGE - 1, true) == int64_t{-1} );

      data.fill(0xFF);
      data[0] = 0xFE;
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), RANGE    , true) == VALUE );
      REQUIRE( cs::toIntegralFromLE<int64_t>(data.data(), RANGE - 1, true) == VALUE );
    }
  }

  TEST_CASE("Deserialize to unsigned integral value.", "[unsigned]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint8_t,8> data;

    { // 8bit
      constexpr uint8_t     VALUE{0x02};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromBE<uint8_t>(data.data(), RANGE) == VALUE );

      data.fill(0);
      data[0] = 2;
      REQUIRE( cs::toIntegralFromLE<uint8_t>(data.data(), RANGE) == VALUE );
    }

    { // 16bit
      constexpr uint16_t    VALUE{0x0102};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 1;
      data[1] = 2;
      REQUIRE( cs::toIntegralFromBE<uint16_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromBE<uint16_t>(data.data(), RANGE - 1) == uint16_t{0x01} );

      data.fill(0);
      data[0] = 2;
      data[1] = 1;
      REQUIRE( cs::toIntegralFromLE<uint16_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromLE<uint16_t>(data.data(), RANGE - 1) == uint16_t{0x02} );
    }

    { // 32bit
      constexpr uint32_t    VALUE{0x01000002};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 1;
      data[3] = 2;
      REQUIRE( cs::toIntegralFromBE<uint32_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromBE<uint32_t>(data.data(), RANGE - 1) == uint32_t{0x010000} );

      data.fill(0);
      data[0] = 2;
      data[3] = 1;
      REQUIRE( cs::toIntegralFromLE<uint32_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromLE<uint32_t>(data.data(), RANGE - 1) == uint32_t{0x000002} );
    }

    { // 64bit
      constexpr uint64_t    VALUE{0x0100000000000002};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      data[0] = 1;
      data[7] = 2;
      REQUIRE( cs::toIntegralFromBE<uint64_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromBE<uint64_t>(data.data(), RANGE - 1) == uint64_t{0x01000000000000} );

      data.fill(0);
      data[0] = 2;
      data[7] = 1;
      REQUIRE( cs::toIntegralFromLE<uint64_t>(data.data(), RANGE    ) == VALUE );
      REQUIRE( cs::toIntegralFromLE<uint64_t>(data.data(), RANGE - 1) == uint64_t{0x00000000000002} );
    }
  }

} // namespace test_deserialize

namespace test_serialize {

  using Buffer = std::array<uint8_t,8>;

  constexpr bool cmpEQ(const Buffer& test, const Buffer& ref, const std::size_t count)
  {
    return 0 < count  &&  count <= test.size()
        ? std::equal(test.data(), test.data() + count, ref.data())
        : false;
  }

  TEST_CASE("Serialize to signed integral value.", "[signed]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    Buffer data;

    { // 8bit
      constexpr int8_t      VALUE{-127};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
    }

    { // 16bit
      constexpr int16_t     VALUE{-32382};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x81, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesBE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x82, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesLE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
    }

    { // 32bit
      constexpr int32_t     VALUE{-2122153084};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x81, 0x82, 0x83, 0x84, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesBE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x82, 0x83, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x84, 0x83, 0x82, 0x81, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesLE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x84, 0x83, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
    }

    { // 64bit
      constexpr int64_t     VALUE{-9114578090645354616};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88}, RANGE) );
      data.fill(0);
      cs::toBytesBE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81}, RANGE) );
      data.fill(0);
      cs::toBytesLE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x00}, RANGE) );
    }
  }

  TEST_CASE("Serialize to unsigned integral value.", "[unsigned]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    Buffer data;

    { // 8bit
      constexpr uint8_t     VALUE{0x01};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
    }

    { // 16bit
      constexpr uint16_t    VALUE{0x0102};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesBE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesLE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
    }

    { // 32bit
      constexpr uint32_t    VALUE{0x01020304};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesBE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x04, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00}, RANGE) );
      data.fill(0);
      cs::toBytesLE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x04, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00}, RANGE) );
    }

    { // 64bit
      constexpr uint64_t    VALUE{0x0102030405060708};
      constexpr std::size_t RANGE = sizeof(VALUE);

      data.fill(0);
      cs::toBytesBE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}, RANGE) );
      data.fill(0);
      cs::toBytesBE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00}, RANGE) );

      data.fill(0);
      cs::toBytesLE(data.data(), RANGE, VALUE);
      REQUIRE( cmpEQ(data, {0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01}, RANGE) );
      data.fill(0);
      cs::toBytesLE(data.data(), RANGE - 1, VALUE);
      REQUIRE( cmpEQ(data, {0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x00}, RANGE) );
    }
  }

} // namespace test_serialize
