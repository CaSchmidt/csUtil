#include "cs/Core/Container.h"
#include <algorithm>
#include <iostream>

#include <catch.hpp>

#include <cs/Core/ByteArray.h>
#include <cs/Crypto/Hash.h>
#include <cs/Text/StringAlgorithm.h>

#undef HAVE_MESSAGE_XL

/*
 * "Test vectors for SHA-1, SHA-2 and SHA-3", cf.
 * https://www.di-mgt.com.au/sha_testvectors.html
 */

namespace test_util {

  constexpr std::size_t sizeOfHash(const cs::Hash::Function func)
  {
    if(        func == cs::Hash::CRC32 ) {
      return cs::Hash::Size_CRC32;
    } else if( func == cs::Hash::MD5 ) {
      return cs::Hash::Size_MD5;
    } else if( func == cs::Hash::SHA1 ) {
      return cs::Hash::Size_SHA1;
    } else if( func == cs::Hash::SHA224 ) {
      return cs::Hash::Size_SHA224;
    } else if( func == cs::Hash::SHA256 ) {
      return cs::Hash::Size_SHA256;
    } else if( func == cs::Hash::SHA384 ) {
      return cs::Hash::Size_SHA384;
    } else if( func == cs::Hash::SHA512 ) {
      return cs::Hash::Size_SHA512;
    }
    return 0;
  }

  template<cs::Hash::Function FUNC>
  using Digest = cs::ByteArray<sizeOfHash(FUNC)>;

  template<cs::Hash::Function FUNC>
  bool test_digest(const std::string& message, const Digest<FUNC>& digest)
  {
    cs::Hash hash{FUNC};

    hash(message.data(), message.size());
    const cs::Buffer result = hash.result();

    return std::equal(result.begin(), result.end(), digest.begin());
  }

  std::string makeMessage1M()
  {
    std::string s;
    cs::resize(&s, 1'000'000, 'a');
    return s;
  }

#ifdef HAVE_MESSAGE_XL
  std::string makeMessageXL()
  {
    constexpr std::size_t COUNT = 0x1000000;

    const char *msg = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";

    std::string s;
    s.reserve(COUNT);

    for(std::size_t i = 0; i < COUNT; i++) {
      s.append(msg);
    }

    return s;
  }
#endif

} // namespace test_util

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
    REQUIRE( test_digest(cs::ByteArray<4>{0x00, 0x00, 0x00, 0x00},
                         digest_No2) );

    const Digest digest_No3{0x24, 0xAB, 0x9D, 0x77};
    REQUIRE( test_digest(cs::ByteArray<3>{0xF2, 0x01, 0x83},
                         digest_No3) );

    const Digest digest_No4{0xB6, 0xC9, 0xB2, 0x87};
    REQUIRE( test_digest(cs::ByteArray<4>{0x0F, 0xAA, 0x00, 0x55},
                         digest_No4) );

    const Digest digest_No5{0x32, 0xA0, 0x62, 0x12};
    REQUIRE( test_digest(cs::ByteArray<4>{0x00, 0xFF, 0x55, 0x11},
                         digest_No5) );

    const Digest digest_No6{0xB0, 0xAE, 0x86, 0x3D};
    REQUIRE( test_digest(cs::ByteArray<9>{0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
                         digest_No6) );

    const Digest digest_No7{0x9C, 0xDE, 0xA2, 0x9B};
    REQUIRE( test_digest(cs::ByteArray<3>{0x92, 0x6B, 0x55},
                         digest_No7) );

    const Digest digest_No8{0xFF, 0xFF, 0xFF, 0xFF};
    REQUIRE( test_digest(cs::ByteArray<4>{0xFF, 0xFF, 0xFF, 0xFF},
                         digest_No8) );
  }

} // namespace test_crc32

namespace test_md5 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::MD5;

  using Digest = test_util::Digest<FUNC>;

  TEST_CASE("Compute MD5 message digests.", "[md5]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Digest digest_No1{0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};
    REQUIRE( test_digest<FUNC>("",
                               digest_No1) );

    const Digest digest_No2{0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61};
    REQUIRE( test_digest<FUNC>("a",
                               digest_No2) );

    const Digest digest_No3{0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72};
    REQUIRE( test_digest<FUNC>("abc",
                               digest_No3) );

    const Digest digest_No4{0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d, 0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0};
    REQUIRE( test_digest<FUNC>("message digest",
                               digest_No4) );

    const Digest digest_No5{0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00, 0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b};
    REQUIRE( test_digest<FUNC>("abcdefghijklmnopqrstuvwxyz",
                               digest_No5) );

    const Digest digest_No6{0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5, 0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f};
    REQUIRE( test_digest<FUNC>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
                               digest_No6) );

    const Digest digest_No7{0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55, 0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a};
    REQUIRE( test_digest<FUNC>("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
                               digest_No7) );
  }

} // namespace test_md5

namespace test_sha1 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::SHA1;

  using Digest = test_util::Digest<FUNC>;

  TEST_CASE("Compute SHA-1 message digests.", "[sha1]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const Digest digest_No1{0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};
    REQUIRE( test_digest<FUNC>("abc",
                               digest_No1) );

    const Digest digest_No2{0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09};
    REQUIRE( test_digest<FUNC>("",
                               digest_No2) );

    const Digest digest_No3{0x84, 0x98, 0x3e, 0x44, 0x1c, 0x3b, 0xd2, 0x6e, 0xba, 0xae, 0x4a, 0xa1, 0xf9, 0x51, 0x29, 0xe5, 0xe5, 0x46, 0x70, 0xf1};
    REQUIRE( test_digest<FUNC>("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                               digest_No3) );

    const Digest digest_No4{0xa4, 0x9b, 0x24, 0x46, 0xa0, 0x2c, 0x64, 0x5b, 0xf4, 0x19, 0xf9, 0x95, 0xb6, 0x70, 0x91, 0x25, 0x3a, 0x04, 0xa2, 0x59};
    REQUIRE( test_digest<FUNC>("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                               digest_No4) );

    const Digest digest_No5{0x34, 0xaa, 0x97, 0x3c, 0xd4, 0xc4, 0xda, 0xa4, 0xf6, 0x1e, 0xeb, 0x2b, 0xdb, 0xad, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6f};
    REQUIRE( test_digest<FUNC>(makeMessage1M(),
                               digest_No5) );

#ifdef HAVE_MESSAGE_XL
    printf("Make message XL...\n"); fflush(stdout);
    const std::string message_No6 = makeMessageXL();
    const Digest digest_No6{0x77, 0x89, 0xf0, 0xc9, 0xef, 0x7b, 0xfc, 0x40, 0xd9, 0x33, 0x11, 0x14, 0x3d, 0xfb, 0xe6, 0x9e, 0x20, 0x17, 0xf5, 0x92};
    printf("Hash message XL...\n"); fflush(stdout);
    REQUIRE( test_digest<FUNC>(message_No6,
                               digest_No6) );
#endif
  }

} // namespace test_sha1
