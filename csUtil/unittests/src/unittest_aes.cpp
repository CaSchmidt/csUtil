#include <algorithm>
#include <array>
#include <charconv>
#include <iostream>

#include <catch.hpp>

#include <cs/Convert/BufferUtil.h>
#include "internal/AesNi.h"

/*
 * "Advanced Encryption Standard (AES) New Instructions Set", Shay Gueron
 *
 * "Cryptographic Algorithm Validation Program", cf.
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/block-ciphers
 */

constexpr std::size_t AES_BLKSIZE = 16;
constexpr std::size_t AES_Nb = 4;

namespace test_aeskeyexp {

  bool cmp_key(const std::size_t i, const uint32_t *w, const char *hexref)
  {
    const std::string s = cs::toString(&w[i*AES_Nb], AES_BLKSIZE);
    return s == hexref;
  }

  TEST_CASE("AES key expansion.", "[aeskeyexp]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint32_t,60> schedule;

    {
      const std::array<cs::byte_t,16> key128 = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
      };

      schedule.fill(0);
      cs::impl_aes::KeyExpansion<128,0>::start(schedule.data(), key128.data());

      REQUIRE( cmp_key( 0, schedule.data(), "2b7e151628aed2a6abf7158809cf4f3c") );
      REQUIRE( cmp_key( 1, schedule.data(), "a0fafe1788542cb123a339392a6c7605") );
      REQUIRE( cmp_key( 2, schedule.data(), "f2c295f27a96b9435935807a7359f67f") );
      REQUIRE( cmp_key( 3, schedule.data(), "3d80477d4716fe3e1e237e446d7a883b") );
      REQUIRE( cmp_key( 4, schedule.data(), "ef44a541a8525b7fb671253bdb0bad00") );
      REQUIRE( cmp_key( 5, schedule.data(), "d4d1c6f87c839d87caf2b8bc11f915bc") );
      REQUIRE( cmp_key( 6, schedule.data(), "6d88a37a110b3efddbf98641ca0093fd") );
      REQUIRE( cmp_key( 7, schedule.data(), "4e54f70e5f5fc9f384a64fb24ea6dc4f") );
      REQUIRE( cmp_key( 8, schedule.data(), "ead27321b58dbad2312bf5607f8d292f") );
      REQUIRE( cmp_key( 9, schedule.data(), "ac7766f319fadc2128d12941575c006e") );
      REQUIRE( cmp_key(10, schedule.data(), "d014f9a8c9ee2589e13f0cc8b6630ca6") );
    }

    {
      const std::array<cs::byte_t,24> key192 = {
        0x8e, 0x73, 0xb0, 0xf7,
        0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b,
        0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2,
        0x52, 0x2c, 0x6b, 0x7b
      };

      schedule.fill(0);
      cs::impl_aes::KeyExpansion<192,0>::start(schedule.data(), key192.data());

      REQUIRE( cmp_key( 0, schedule.data(), "8e73b0f7da0e6452c810f32b809079e5") );
      REQUIRE( cmp_key( 1, schedule.data(), "62f8ead2522c6b7bfe0c91f72402f5a5") );
      REQUIRE( cmp_key( 2, schedule.data(), "ec12068e6c827f6b0e7a95b95c56fec2") );
      REQUIRE( cmp_key( 3, schedule.data(), "4db7b4bd69b5411885a74796e92538fd") );
      REQUIRE( cmp_key( 4, schedule.data(), "e75fad44bb095386485af05721efb14f") );
      REQUIRE( cmp_key( 5, schedule.data(), "a448f6d94d6dce24aa326360113b30e6") );
      REQUIRE( cmp_key( 6, schedule.data(), "a25e7ed583b1cf9a27f939436a94f767") );
      REQUIRE( cmp_key( 7, schedule.data(), "c0a69407d19da4e1ec1786eb6fa64971") );
      REQUIRE( cmp_key( 8, schedule.data(), "485f703222cb8755e26d135233f0b7b3") );
      REQUIRE( cmp_key( 9, schedule.data(), "40beeb282f18a2596747d26b458c553e") );
      REQUIRE( cmp_key(10, schedule.data(), "a7e1466c9411f1df821f750aad07d753") );
      REQUIRE( cmp_key(11, schedule.data(), "ca4005388fcc5006282d166abc3ce7b5") );
      // REQUIRE( cmp_key(12, schedule.data(), "") );
    }
  }

} // namespace test_aeskeyexp
