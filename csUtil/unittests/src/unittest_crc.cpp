#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch.hpp>

#include <cs/Core/ByteArray.h>
#include <cs/Core/CRC.h>

#define HAVE_CRC8
#define HAVE_CRC16
#define HAVE_CRC32
#define HAVE_CRC64

namespace test_data {

  const cs::ByteArray<9> data0{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  const cs::ByteArray<4> data1{0x00, 0x00, 0x00, 0x00};
  const cs::ByteArray<3> data2{0xF2, 0x01, 0x83};
  const cs::ByteArray<4> data3{0x0F, 0xAA, 0x00, 0x55};
  const cs::ByteArray<4> data4{0x00, 0xFF, 0x55, 0x11};
  const cs::ByteArray<9> data5{0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  const cs::ByteArray<3> data6{0x92, 0x6B, 0x55};
  const cs::ByteArray<4> data7{0xFF, 0xFF, 0xFF, 0xFF};

} // namespace test_data

#ifdef HAVE_CRC8
namespace test_crc8 {

  using namespace test_data;

  TEST_CASE("Crc_CalculateCRC8() results.", "[crc8]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC8 = cs::CRC<uint8_t,0x1D,0xFF,0xFF,true,true>;
    using crc_t = CRC8::value_type;

    const CRC8 crc8;

    REQUIRE( crc8(data0.data(), data0.size()) == crc_t{0x4B} );
    REQUIRE( crc8(data1.data(), data1.size()) == crc_t{0x59} );
    REQUIRE( crc8(data2.data(), data2.size()) == crc_t{0x37} );
    REQUIRE( crc8(data3.data(), data3.size()) == crc_t{0x79} );
    REQUIRE( crc8(data4.data(), data4.size()) == crc_t{0xB8} );
    REQUIRE( crc8(data5.data(), data5.size()) == crc_t{0xCB} );
    REQUIRE( crc8(data6.data(), data6.size()) == crc_t{0x8C} );
    REQUIRE( crc8(data7.data(), data7.size()) == crc_t{0x74} );
  }

  TEST_CASE("Crc_CalculateCRC8H2F() results.", "[crc8]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC8 = cs::CRC<uint8_t,0x2F,0xFF,0xFF,true,true>;
    using crc_t = CRC8::value_type;

    const CRC8 crc8;

    REQUIRE( crc8(data0.data(), data0.size()) == crc_t{0xDF} );
    REQUIRE( crc8(data1.data(), data1.size()) == crc_t{0x12} );
    REQUIRE( crc8(data2.data(), data2.size()) == crc_t{0xC2} );
    REQUIRE( crc8(data3.data(), data3.size()) == crc_t{0xC6} );
    REQUIRE( crc8(data4.data(), data4.size()) == crc_t{0x77} );
    REQUIRE( crc8(data5.data(), data5.size()) == crc_t{0x11} );
    REQUIRE( crc8(data6.data(), data6.size()) == crc_t{0x33} );
    REQUIRE( crc8(data7.data(), data7.size()) == crc_t{0x6C} );
  }

} // namespace test_crc8
#endif

#ifdef HAVE_CRC16
namespace test_crc16 {

  using namespace test_data;

  TEST_CASE("Crc_CalculateCRC16() results.", "[crc16]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC16 = cs::CRC<uint16_t,0x1021,0xFFFF,0x0,true,true>;
    using crc_t = CRC16::value_type;

    const CRC16 crc16;

    REQUIRE( crc16(data0.data(), data0.size()) == crc_t{0x29B1} );
    REQUIRE( crc16(data1.data(), data1.size()) == crc_t{0x84C0} );
    REQUIRE( crc16(data2.data(), data2.size()) == crc_t{0xD374} );
    REQUIRE( crc16(data3.data(), data3.size()) == crc_t{0x2023} );
    REQUIRE( crc16(data4.data(), data4.size()) == crc_t{0xB8F9} );
    REQUIRE( crc16(data5.data(), data5.size()) == crc_t{0xF53F} );
    REQUIRE( crc16(data6.data(), data6.size()) == crc_t{0x0745} );
    REQUIRE( crc16(data7.data(), data7.size()) == crc_t{0x1D0F} );
  }

  TEST_CASE("Crc_CalculateCRC16ARC() results.", "[crc16]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC16 = cs::CRC<uint16_t,0x8005,0x0,0x0>;
    using crc_t = CRC16::value_type;

    const CRC16 crc16;

    REQUIRE( crc16(data0.data(), data0.size()) == crc_t{0xBB3D} );
    REQUIRE( crc16(data1.data(), data1.size()) == crc_t{0x0000} );
    REQUIRE( crc16(data2.data(), data2.size()) == crc_t{0xC2E1} );
    REQUIRE( crc16(data3.data(), data3.size()) == crc_t{0x0BE3} );
    REQUIRE( crc16(data4.data(), data4.size()) == crc_t{0x6CCF} );
    REQUIRE( crc16(data5.data(), data5.size()) == crc_t{0xAE98} );
    REQUIRE( crc16(data6.data(), data6.size()) == crc_t{0xE24E} );
    REQUIRE( crc16(data7.data(), data7.size()) == crc_t{0x9401} );
  }

} // namespace test_crc16
#endif

#ifdef HAVE_CRC32
namespace test_crc32 {

  using namespace test_data;

  TEST_CASE("Crc_CalculateCRC32() results.", "[crc32]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC32 = cs::CRC32;
    using crc_t = CRC32::value_type;

    const CRC32 crc32;

    REQUIRE( crc32(data0.data(), data0.size()) == crc_t{0xCBF43926} );
    REQUIRE( crc32(data1.data(), data1.size()) == crc_t{0x2144DF1C} );
    REQUIRE( crc32(data2.data(), data2.size()) == crc_t{0x24AB9D77} );
    REQUIRE( crc32(data3.data(), data3.size()) == crc_t{0xB6C9B287} );
    REQUIRE( crc32(data4.data(), data4.size()) == crc_t{0x32A06212} );
    REQUIRE( crc32(data5.data(), data5.size()) == crc_t{0xB0AE863D} );
    REQUIRE( crc32(data6.data(), data6.size()) == crc_t{0x9CDEA29B} );
    REQUIRE( crc32(data7.data(), data7.size()) == crc_t{0xFFFFFFFF} );
  }

  TEST_CASE("Crc_CalculateCRC32P4() results.", "[crc32]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC32 = cs::CRC<uint32_t,0xF4ACFB13,0xFFFFFFFF,0xFFFFFFFF>;
    using crc_t = CRC32::value_type;

    const CRC32 crc32;

    REQUIRE( crc32(data0.data(), data0.size()) == crc_t{0x1697D06A} );
    REQUIRE( crc32(data1.data(), data1.size()) == crc_t{0x6FB32240} );
    REQUIRE( crc32(data2.data(), data2.size()) == crc_t{0x4F721A25} );
    REQUIRE( crc32(data3.data(), data3.size()) == crc_t{0x20662DF8} );
    REQUIRE( crc32(data4.data(), data4.size()) == crc_t{0x9BD7996E} );
    REQUIRE( crc32(data5.data(), data5.size()) == crc_t{0xA65A343D} );
    REQUIRE( crc32(data6.data(), data6.size()) == crc_t{0xEE688A78} );
    REQUIRE( crc32(data7.data(), data7.size()) == crc_t{0xFFFFFFFF} );
  }

} // namespace test_crc32
#endif

#ifdef HAVE_CRC64
namespace test_crc64 {

  using namespace test_data;

  TEST_CASE("Crc_CalculateCRC64() results.", "[crc64]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using CRC64 = cs::CRC<uint64_t,0x42F0E1EBA9EA3693,0xFFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFFF>;
    using crc_t = CRC64::value_type;

    const CRC64 crc64;

    REQUIRE( crc64(data0.data(), data0.size()) == crc_t{0x995DC9BBDF1939FA} );
    REQUIRE( crc64(data1.data(), data1.size()) == crc_t{0xF4A586351E1B9F4B} );
    REQUIRE( crc64(data2.data(), data2.size()) == crc_t{0x319C27668164F1C6} );
    REQUIRE( crc64(data3.data(), data3.size()) == crc_t{0x54C5D0F7667C1575} );
    REQUIRE( crc64(data4.data(), data4.size()) == crc_t{0xA63822BE7E0704E6} );
    REQUIRE( crc64(data5.data(), data5.size()) == crc_t{0x701ECEB219A8E5D5} );
    REQUIRE( crc64(data6.data(), data6.size()) == crc_t{0x5FAA96A9B59F3E4E} );
    REQUIRE( crc64(data7.data(), data7.size()) == crc_t{0xFFFFFFFF00000000} );
  }

} // namespace test_crc64
#endif
