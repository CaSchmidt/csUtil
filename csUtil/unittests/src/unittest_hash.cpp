#include <algorithm>
#include <iostream>

#include <catch.hpp>

#include <cs/Core/ByteArray.h>
#include <cs/Crypto/Hash.h>
#include <cs/Text/StringAlgorithm.h>

namespace test_crc32 {

  using Digest = cs::ByteArray<cs::Hash::Size_CRC32>;

  template<std::size_t N>
  bool test_digest(const cs::ByteArray<N>& message, const Digest& ref)
  {
    cs::Hash hash{cs::Hash::CRC32};

    hash(message.data(), message.size());
    const cs::Buffer digest = hash.result();

    return std::equal(digest.begin(), digest.end(), ref.begin());
  }

  TEST_CASE("Compute CRC-32 message digests.", "[crc32]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Digest digest_No1{0xCB, 0xF4, 0x39, 0x26};
    REQUIRE( test_digest(cs::ByteArray<9>{'1', '2', '3', '4', '5', '6', '7', '8', '9'},
                         digest_No1) );

    const Digest digest_No2{0x21, 0x44, 0xDF, 0x1C};
    REQUIRE( test_digest(cs::ByteArray<4>{0x00, 0x00, 0x00, 0x00}, digest_No2) );

    const Digest digest_No3{0x24, 0xAB, 0x9D, 0x77};
    REQUIRE( test_digest(cs::ByteArray<3>{0xF2, 0x01, 0x83}, digest_No3) );

    const Digest digest_No4{0xB6, 0xC9, 0xB2, 0x87};
    REQUIRE( test_digest(cs::ByteArray<4>{0x0F, 0xAA, 0x00, 0x55}, digest_No4) );

    const Digest digest_No5{0x32, 0xA0, 0x62, 0x12};
    REQUIRE( test_digest(cs::ByteArray<4>{0x00, 0xFF, 0x55, 0x11}, digest_No5) );

    const Digest digest_No6{0xB0, 0xAE, 0x86, 0x3D};
    REQUIRE( test_digest(cs::ByteArray<9>{0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
                         digest_No6) );

    const Digest digest_No7{0x9C, 0xDE, 0xA2, 0x9B};
    REQUIRE( test_digest(cs::ByteArray<3>{0x92, 0x6B, 0x55}, digest_No7) );

    const Digest digest_No8{0xFF, 0xFF, 0xFF, 0xFF};
    REQUIRE( test_digest(cs::ByteArray<4>{0xFF, 0xFF, 0xFF, 0xFF}, digest_No8) );
  }

} // namespace test_crc32

namespace test_md5 {

  using Digest = cs::ByteArray<cs::Hash::Size_MD5>;

  bool test_digest(const char *message, const Digest& ref)
  {
    cs::Hash hash{cs::Hash::MD5};

    hash(message, cs::length(message));
    const cs::Buffer digest = hash.result();

    return std::equal(digest.begin(), digest.end(), ref.begin());
  }

  TEST_CASE("Compute MD5 message digests.", "[md5]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Digest digest_No1{0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};
    REQUIRE( test_digest("", digest_No1) );

    const Digest digest_No2{0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61};
    REQUIRE( test_digest("a", digest_No2) );

    const Digest digest_No3{0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72};
    REQUIRE( test_digest("abc", digest_No3) );

    const Digest digest_No4{0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d, 0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0};
    REQUIRE( test_digest("message digest", digest_No4) );

    const Digest digest_No5{0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00, 0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b};
    REQUIRE( test_digest("abcdefghijklmnopqrstuvwxyz", digest_No5) );

    const Digest digest_No6{0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5, 0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f};
    REQUIRE( test_digest("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
                         digest_No6) );

    const Digest digest_No7{0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55, 0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a};
    REQUIRE( test_digest("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
                         digest_No7) );
  }

} // namespace test_md5
