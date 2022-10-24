#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch.hpp>

#include <cs/Core/CRC.h>

namespace test_data {

  const std::array<uint8_t,9> data0{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  const std::array<uint8_t,4> data1{0x00, 0x00, 0x00, 0x00};
  const std::array<uint8_t,3> data2{0xF2, 0x01, 0x83};
  const std::array<uint8_t,4> data3{0x0F, 0xAA, 0x00, 0x55};
  const std::array<uint8_t,4> data4{0x00, 0xFF, 0x55, 0x11};
  const std::array<uint8_t,9> data5{0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  const std::array<uint8_t,3> data6{0x92, 0x6B, 0x55};
  const std::array<uint8_t,4> data7{0xFF, 0xFF, 0xFF, 0xFF};

} // namespace test_data

namespace test_crc32 {

  using namespace test_data;

  using crc_t = cs::CRC32::value_type;

  TEST_CASE("Crc_CalculateCRC32() results.", "[crc32]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::CRC32 crc32(0x04C11DB7);

    REQUIRE( crc32(data0.data(), data0.size()) == crc_t(0xCBF43926) );
    REQUIRE( crc32(data1.data(), data1.size()) == crc_t(0x2144DF1C) );
    REQUIRE( crc32(data2.data(), data2.size()) == crc_t(0x24AB9D77) );
    REQUIRE( crc32(data3.data(), data3.size()) == crc_t(0xB6C9B287) );
    REQUIRE( crc32(data4.data(), data4.size()) == crc_t(0x32A06212) );
    REQUIRE( crc32(data5.data(), data5.size()) == crc_t(0xB0AE863D) );
    REQUIRE( crc32(data6.data(), data6.size()) == crc_t(0x9CDEA29B) );
    REQUIRE( crc32(data7.data(), data7.size()) == crc_t(0xFFFFFFFF) );
  }

  TEST_CASE("Crc_CalculateCRC32P4() results.", "[crc32]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::CRC32 crc32(0xF4ACFB13);

    REQUIRE( crc32(data0.data(), data0.size()) == crc_t(0x1697D06A) );
    REQUIRE( crc32(data1.data(), data1.size()) == crc_t(0x6FB32240) );
    REQUIRE( crc32(data2.data(), data2.size()) == crc_t(0x4F721A25) );
    REQUIRE( crc32(data3.data(), data3.size()) == crc_t(0x20662DF8) );
    REQUIRE( crc32(data4.data(), data4.size()) == crc_t(0x9BD7996E) );
    REQUIRE( crc32(data5.data(), data5.size()) == crc_t(0xA65A343D) );
    REQUIRE( crc32(data6.data(), data6.size()) == crc_t(0xEE688A78) );
    REQUIRE( crc32(data7.data(), data7.size()) == crc_t(0xFFFFFFFF) );
  }

} // namespace test_crc32
