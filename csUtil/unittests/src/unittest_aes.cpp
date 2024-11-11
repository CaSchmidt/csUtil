#include <array>
#include <iostream>
#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Convert/BufferUtil.h>
#include <cs/Core/Container.h>
#include <cs/Crypto/BlockCipher.h>
#include <cs/System/PathFormatter.h>
#include <cs/Text/StringUtil.h>
#include <cs/Text/TextIO.h>

#include "internal/AesNi.h"

/*
 * "Advanced Encryption Standard (AES) New Instructions Set", Shay Gueron
 *
 * "Cryptographic Algorithm Validation Program", cf.
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/block-ciphers
 */

constexpr std::size_t AES_BLKSIZE = 16;
constexpr std::size_t AES_Nb = 4;

namespace test_util {

  bool is_cavp_data(const cs::BlockCipherPtr& algo, const cs::Buffer& key,
                    const cs::Buffer& cipher, const cs::Buffer& plain)
  {
    return
        algo                                &&
        algo->keySize()   == key.size()     &&
        algo->blockSize() == cipher.size()  &&
        algo->blockSize() == plain.size();
  }

  bool test_cavp(const cs::BlockCipher::Algorithm algoid,
                 const std::filesystem::path& filename)
  {
    cs::BlockCipherPtr algo = cs::BlockCipher::make(algoid);
    cs::Buffer temp;

    const std::list<std::string> lines = cs::readLines(filename, cs::LineFlag::All);
    if( lines.empty()  ||  !algo  ||  !cs::resize(temp, algo->blockSize()) ) {
      return false;
    }

    cs::size_t cntDecrypt = 0, cntDecryptOK = 0;
    cs::size_t cntEncrypt = 0, cntEncryptOK = 0;
    cs::Buffer key, cipher, plain;
    int mode = 0;
    for(const std::string& line : lines) {
      const char *prefix_COUNT = "COUNT = ";
      const char *prefix_KEY = "KEY = ";
      const char *prefix_PLAIN = "PLAINTEXT = ";
      const char *prefix_CIPHER = "CIPHERTEXT = ";

      if(        line == "[ENCRYPT]" ) {
        mode = 1;
        continue;
      } else if( line == "[DECRYPT]" ) {
        mode = 2;
        continue;
      }

      if( cs::startsWith(line, prefix_COUNT) ) {
        if(        mode == 1 ) { // encrypt
          cntEncrypt++;
        } else if( mode == 2 ) { // decrypt
          cntDecrypt++;
        }
        key.clear();
        cipher.clear();
        plain.clear();
        continue;
      } else if( cs::startsWith(line, prefix_KEY) ) {
        key = cs::toBuffer(line.substr(cs::strlen(prefix_KEY)));
      } else if( cs::startsWith(line, prefix_PLAIN) ) {
        plain = cs::toBuffer(line.substr(cs::strlen(prefix_PLAIN)));
      } else if( cs::startsWith(line, prefix_CIPHER) ) {
        cipher = cs::toBuffer(line.substr(cs::strlen(prefix_CIPHER)));
      } else {
        continue;
      }

      if( is_cavp_data(algo, key, cipher, plain) ) {
        algo->setKey(key.data());
        if(        mode == 1 ) { // encrypt
          algo->encryptBlock(temp.data(), plain.data());
          if( temp == cipher ) {
            cntEncryptOK++;
          }

        } else if( mode == 2 ) { // decrypt
          algo->decryptBlock(temp.data(), cipher.data());
          if( temp == plain ) {
            cntDecryptOK++;
          }
        }
      } // Valid Data
    } // For Each Line

    std::println("AES-{}: {}", algo->keySize()*8, filename.filename());
    std::println("decrypt: {}/{}", cntDecryptOK, cntDecrypt);
    std::println("encrypt: {}/{}", cntEncryptOK, cntEncrypt);

    return cntDecrypt == cntDecryptOK  &&  cntEncrypt ==  cntEncryptOK;
  }

} // namespace test_util

namespace test_aeskeyexp {

  bool cmp_key(const std::size_t i, const uint32_t *w, const char *hexref)
  {
    const std::string s = cs::toString(&w[i*AES_Nb], AES_BLKSIZE);
    return s == hexref;
  }

  TEST_CASE("AES key expansion.", "[aeskeyexp]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    std::array<uint32_t,60> schedule;

    { // AES-128 /////////////////////////////////////////////////////////////
      using Traits = cs::impl_aes::AesTraits<128>;

      const std::array<cs::byte_t,16> key128 = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
      };

      schedule.fill(0);
      cs::impl_aes::KeyExpansion<Traits,0>::run(schedule.data(), key128.data());

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

    { // AES-192 /////////////////////////////////////////////////////////////
      using Traits = cs::impl_aes::AesTraits<192>;

      const std::array<cs::byte_t,24> key192 = {
        0x8e, 0x73, 0xb0, 0xf7,
        0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b,
        0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2,
        0x52, 0x2c, 0x6b, 0x7b
      };

      schedule.fill(0);
      cs::impl_aes::KeyExpansion<Traits,0>::run(schedule.data(), key192.data());

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
      REQUIRE( cmp_key(12, schedule.data(), "e98ba06f448c773c8ecc720401002202") );
    }

    { // AES-256 /////////////////////////////////////////////////////////////
      using Traits = cs::impl_aes::AesTraits<256>;

      const std::array<cs::byte_t,32> key256 = {
        0x60, 0x3d, 0xeb, 0x10,
        0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0,
        0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07,
        0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3,
        0x09, 0x14, 0xdf, 0xf4
      };

      schedule.fill(0);
      cs::impl_aes::KeyExpansion<Traits,0>::run(schedule.data(), key256.data());

      REQUIRE( cmp_key( 0, schedule.data(), "603deb1015ca71be2b73aef0857d7781") );
      REQUIRE( cmp_key( 1, schedule.data(), "1f352c073b6108d72d9810a30914dff4") );
      REQUIRE( cmp_key( 2, schedule.data(), "9ba354118e6925afa51a8b5f2067fcde") );
      REQUIRE( cmp_key( 3, schedule.data(), "a8b09c1a93d194cdbe49846eb75d5b9a") );
      REQUIRE( cmp_key( 4, schedule.data(), "d59aecb85bf3c917fee94248de8ebe96") );
      REQUIRE( cmp_key( 5, schedule.data(), "b5a9328a2678a647983122292f6c79b3") );
      REQUIRE( cmp_key( 6, schedule.data(), "812c81addadf48ba24360af2fab8b464") );
      REQUIRE( cmp_key( 7, schedule.data(), "98c5bfc9bebd198e268c3ba709e04214") );
      REQUIRE( cmp_key( 8, schedule.data(), "68007bacb2df331696e939e46c518d80") );
      REQUIRE( cmp_key( 9, schedule.data(), "c814e20476a9fb8a5025c02d59c58239") );
      REQUIRE( cmp_key(10, schedule.data(), "de1369676ccc5a71fa2563959674ee15") );
      REQUIRE( cmp_key(11, schedule.data(), "5886ca5d2e2f31d77e0af1fa27cf73c3") );
      REQUIRE( cmp_key(12, schedule.data(), "749c47ab18501ddae2757e4f7401905a") );
      REQUIRE( cmp_key(13, schedule.data(), "cafaaae3e4d59b349adf6acebd10190d") );
      REQUIRE( cmp_key(14, schedule.data(), "fe4890d1e6188d0b046df344706c631e") );
    }
  }

} // namespace test_aeskeyexp

namespace test_aescrypt {

  TEST_CASE("AES de-/encryption.", "[aescrypt]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( test_util::test_cavp(cs::BlockCipher::AES128, "./testdata/ECBVarKey128.rsp") );
    REQUIRE( test_util::test_cavp(cs::BlockCipher::AES128, "./testdata/ECBVarTxt128.rsp") );
    REQUIRE( test_util::test_cavp(cs::BlockCipher::AES192, "./testdata/ECBVarKey192.rsp") );
    REQUIRE( test_util::test_cavp(cs::BlockCipher::AES192, "./testdata/ECBVarTxt192.rsp") );
    REQUIRE( test_util::test_cavp(cs::BlockCipher::AES256, "./testdata/ECBVarKey256.rsp") );
    REQUIRE( test_util::test_cavp(cs::BlockCipher::AES256, "./testdata/ECBVarTxt256.rsp") );
  }

} // namespace test_aescrypt
