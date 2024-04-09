#include <algorithm>
#include <charconv>
#include <iostream>
#include <numeric>

#include <catch2/catch_test_macros.hpp>

#include <cs/Convert/BufferUtil.h>
#include <cs/Core/ByteArray.h>
#include <cs/Core/Container.h>
#include <cs/Crypto/CryptoUtil.h>
#include <cs/IO/File.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>
#include <cs/Text/TextIO.h>

#undef HAVE_MESSAGE_XL

/*
 * "Test vectors for SHA-1, SHA-2 and SHA-3", cf.
 * https://www.di-mgt.com.au/sha_testvectors.html
 *
 * "Cryptographic Algorithm Validation Program", cf.
 * https://csrc.nist.gov/Projects/cryptographic-algorithm-validation-program/Secure-Hashing
 */

namespace test_util {

  bool test_digest(const std::string& message, const cs::Buffer& refDigest,
                   const cs::Hash::Function func)
  {
    cs::HashPtr hash = cs::Hash::make(func);
    /*
     * NOTE: Discarding messages with zero length won't enable
     *       us to compute the digest of empty messages!
     */
    if( !hash  ||  hash->digestSize() != refDigest.size() ) {
      return false;
    }

    if( hash->update(message.data(), message.size()) != message.size() ) {
      return false;
    }
    const cs::Buffer result = hash->digest();

    return std::equal(result.begin(), result.end(), refDigest.begin());
  }

  bool test_digest(const cs::Buffer& buffer, const cs::Buffer& refDigest,
                   const cs::Hash::Function func)
  {
    return test_digest(std::string(reinterpret_cast<const char*>(buffer.data()), buffer.size()),
                       refDigest, func);
  }

  bool test_cavp(const std::filesystem::path& path,
                 const cs::Hash::Function func)
  {
    cs::HashPtr hash = cs::Hash::make(func);
    const std::list<std::string> lines = cs::readLines(path, cs::LineFlag::All);
    if( !hash  ||  lines.empty() ) {
      return false;
    }

    std::size_t cntDigestsOK = 0;
    std::size_t cntMessages  = 0;

    std::size_t length = cs::MAX_SIZE_T;
    cs::Buffer message;
    for(const std::string& line : lines) {
      const char *prefix_Len = "Len = ";
      const char *prefix_Msg = "Msg = ";
      const char *prefix_MD = "MD = ";

      if(        cs::startsWith(line, prefix_Len) ) {
        length = cs::MAX_SIZE_T;

        const std::string value = line.substr(cs::strlen(prefix_Len));

        const std::from_chars_result result =
            std::from_chars(value.data(), value.data() + value.size(), length);
        if( result.ec != std::errc{} ) {
          continue;
        }

        length /= 8;

        cntMessages++;

      } else if( cs::startsWith(line, prefix_Msg) ) {
        message = cs::toBuffer(line.substr(cs::strlen(prefix_Msg)));
        if( length != 0  &&  message.size() != length ) {
          length = cs::MAX_SIZE_T;
          continue;
        }

      } else if( cs::startsWith(line, prefix_MD) ) {
        if( length == cs::MAX_SIZE_T ) {
          continue;
        }

        const cs::Buffer refDigest = cs::toBuffer(line.substr(cs::strlen(prefix_MD)));
        if( refDigest.size() != hash->digestSize() ) {
          length = cs::MAX_SIZE_T;
          continue;
        }

        if( hash->update(message.data(), length) == length ) {
          const cs::Buffer digest = hash->digest();

          if( digest == refDigest ) {
            cntDigestsOK++;
          }
        }

        length = cs::MAX_SIZE_T;
      }
    } // for( lines )

    cs::println("%: %/%", path.filename(), cntDigestsOK, cntMessages);

    return cntDigestsOK == cntMessages;
  }

  std::string makeMessage1M()
  {
    std::string s;
    cs::resize(s, 1'000'000, 'a');
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

  bool test_file_1M(const cs::Buffer& refDigest, const cs::Hash::Function func)
  {
    constexpr cs::size_t SIZ_TEMP = 333;

    const std::filesystem::path filename = "./test_1M.txt";

    cs::println("Compute message digest of file %.", filename.filename());

    // (1) Create Message ////////////////////////////////////////////////////

    const std::string message = makeMessage1M();
    if( message.empty() ) {
      cs::println(std::cerr, "ERROR: Unable to create message!");
      return false;
    }

    // (2) Write Message to File /////////////////////////////////////////////

    {
      cs::File output;
      if( !output.open(filename, cs::FileOpenFlag::Write) ) {
        cs::println(std::cerr, "ERROR: Unable to create file %!", filename);
        return false;
      }

      if( output.write(message.data(), message.size()) != message.size() ) {
        cs::println(std::cerr, "ERROR: Unable to write file %!", filename);
        return false;
      }
    }

    // (3) Open File as Input ////////////////////////////////////////////////

    cs::File input;
    if( !input.open(filename) ) {
      cs::println(std::cerr, "ERROR: Unable to open file %!", filename.filename());
      return false;
    }

    const cs::Buffer digest = cs::sum(input, func, SIZ_TEMP);
    if( digest.empty() ) {
      cs::println(std::cerr, "ERROR: Unable to hash file %!", filename.filename());
      return false;
    }

    return digest == refDigest;
  }

} // namespace test_util

namespace test_crc32 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::CRC32;

  TEST_CASE("Compute CRC-32 message digests.", "[crc32]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0xCB, 0xF4, 0x39, 0x26};
    REQUIRE( test_digest(cs::Buffer{'1', '2', '3', '4', '5', '6', '7', '8', '9'},
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0x21, 0x44, 0xDF, 0x1C};
    REQUIRE( test_digest(cs::Buffer{0x00, 0x00, 0x00, 0x00},
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x24, 0xAB, 0x9D, 0x77};
    REQUIRE( test_digest(cs::Buffer{0xF2, 0x01, 0x83},
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0xB6, 0xC9, 0xB2, 0x87};
    REQUIRE( test_digest(cs::Buffer{0x0F, 0xAA, 0x00, 0x55},
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0x32, 0xA0, 0x62, 0x12};
    REQUIRE( test_digest(cs::Buffer{0x00, 0xFF, 0x55, 0x11},
                         digest_No5, FUNC) );

    const cs::Buffer digest_No6{0xB0, 0xAE, 0x86, 0x3D};
    REQUIRE( test_digest(cs::Buffer{0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
                         digest_No6, FUNC) );

    const cs::Buffer digest_No7{0x9C, 0xDE, 0xA2, 0x9B};
    REQUIRE( test_digest(cs::Buffer{0x92, 0x6B, 0x55},
                         digest_No7, FUNC) );

    const cs::Buffer digest_No8{0xFF, 0xFF, 0xFF, 0xFF};
    REQUIRE( test_digest(cs::Buffer{0xFF, 0xFF, 0xFF, 0xFF},
                         digest_No8, FUNC) );
  }

} // namespace test_crc32

namespace test_md5 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::MD5;

  TEST_CASE("Compute MD5 message digests.", "[md5]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};
    REQUIRE( test_digest("",
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61};
    REQUIRE( test_digest("a",
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72};
    REQUIRE( test_digest("abc",
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d, 0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0};
    REQUIRE( test_digest("message digest",
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00, 0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b};
    REQUIRE( test_digest("abcdefghijklmnopqrstuvwxyz",
                         digest_No5, FUNC) );

    const cs::Buffer digest_No6{0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5, 0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f};
    REQUIRE( test_digest("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
                         digest_No6, FUNC) );

    const cs::Buffer digest_No7{0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55, 0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a};
    REQUIRE( test_digest("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
                         digest_No7, FUNC) );
  }

} // namespace test_md5

namespace test_sha1 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::SHA1;

  TEST_CASE("Compute SHA-1 message digests.", "[sha1]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};
    REQUIRE( test_digest("abc",
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09};
    REQUIRE( test_digest("",
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x84, 0x98, 0x3e, 0x44, 0x1c, 0x3b, 0xd2, 0x6e, 0xba, 0xae, 0x4a, 0xa1, 0xf9, 0x51, 0x29, 0xe5, 0xe5, 0x46, 0x70, 0xf1};
    REQUIRE( test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0xa4, 0x9b, 0x24, 0x46, 0xa0, 0x2c, 0x64, 0x5b, 0xf4, 0x19, 0xf9, 0x95, 0xb6, 0x70, 0x91, 0x25, 0x3a, 0x04, 0xa2, 0x59};
    REQUIRE( test_digest("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0x34, 0xaa, 0x97, 0x3c, 0xd4, 0xc4, 0xda, 0xa4, 0xf6, 0x1e, 0xeb, 0x2b, 0xdb, 0xad, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6f};
    REQUIRE( test_digest(makeMessage1M(),
                         digest_No5, FUNC) );

#ifdef HAVE_MESSAGE_XL
    printf("Make message XL...\n"); fflush(stdout);
    const std::string message_No6 = makeMessageXL();
    const cs::Buffer digest_No6{0x77, 0x89, 0xf0, 0xc9, 0xef, 0x7b, 0xfc, 0x40, 0xd9, 0x33, 0x11, 0x14, 0x3d, 0xfb, 0xe6, 0x9e, 0x20, 0x17, 0xf5, 0x92};
    printf("Hash message XL...\n"); fflush(stdout);
    REQUIRE( test_digest(message_No6,
                         digest_No6, FUNC) );
#endif

    REQUIRE( test_cavp("./testdata/SHA1ShortMsg.rsp", FUNC) );
    REQUIRE( test_cavp("./testdata/SHA1LongMsg.rsp", FUNC) );
    REQUIRE( test_file_1M(digest_No5, FUNC) );
  }

} // namespace test_sha1

namespace test_sha224 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::SHA224;

  TEST_CASE("Compute SHA-224 message digests.", "[sha224]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0x23, 0x09, 0x7d, 0x22, 0x34, 0x05, 0xd8, 0x22, 0x86, 0x42, 0xa4, 0x77, 0xbd, 0xa2, 0x55, 0xb3, 0x2a, 0xad, 0xbc, 0xe4, 0xbd, 0xa0, 0xb3, 0xf7, 0xe3, 0x6c, 0x9d, 0xa7};
    REQUIRE( test_digest("abc",
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0xd1, 0x4a, 0x02, 0x8c, 0x2a, 0x3a, 0x2b, 0xc9, 0x47, 0x61, 0x02, 0xbb, 0x28, 0x82, 0x34, 0xc4, 0x15, 0xa2, 0xb0, 0x1f, 0x82, 0x8e, 0xa6, 0x2a, 0xc5, 0xb3, 0xe4, 0x2f};
    REQUIRE( test_digest("",
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x75, 0x38, 0x8b, 0x16, 0x51, 0x27, 0x76, 0xcc, 0x5d, 0xba, 0x5d, 0xa1, 0xfd, 0x89, 0x01, 0x50, 0xb0, 0xc6, 0x45, 0x5c, 0xb4, 0xf5, 0x8b, 0x19, 0x52, 0x52, 0x25, 0x25};
    REQUIRE( test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0xc9, 0x7c, 0xa9, 0xa5, 0x59, 0x85, 0x0c, 0xe9, 0x7a, 0x04, 0xa9, 0x6d, 0xef, 0x6d, 0x99, 0xa9, 0xe0, 0xe0, 0xe2, 0xab, 0x14, 0xe6, 0xb8, 0xdf, 0x26, 0x5f, 0xc0, 0xb3};
    REQUIRE( test_digest("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0x20, 0x79, 0x46, 0x55, 0x98, 0x0c, 0x91, 0xd8, 0xbb, 0xb4, 0xc1, 0xea, 0x97, 0x61, 0x8a, 0x4b, 0xf0, 0x3f, 0x42, 0x58, 0x19, 0x48, 0xb2, 0xee, 0x4e, 0xe7, 0xad, 0x67};
    REQUIRE( test_digest(makeMessage1M(),
                         digest_No5, FUNC) );

#ifdef HAVE_MESSAGE_XL
    printf("Make message XL...\n"); fflush(stdout);
    const std::string message_No6 = makeMessageXL();
    const cs::Buffer digest_No6{0xb5, 0x98, 0x97, 0x13, 0xca, 0x4f, 0xe4, 0x7a, 0x00, 0x9f, 0x86, 0x21, 0x98, 0x0b, 0x34, 0xe6, 0xd6, 0x3e, 0xd3, 0x06, 0x3b, 0x2a, 0x0a, 0x2c, 0x86, 0x7d, 0x8a, 0x85};
    printf("Hash message XL...\n"); fflush(stdout);
    REQUIRE( test_digest(message_No6,
                         digest_No6, FUNC) );
#endif

    REQUIRE( test_cavp("./testdata/SHA224ShortMsg.rsp", FUNC) );
    REQUIRE( test_cavp("./testdata/SHA224LongMsg.rsp", FUNC) );
    REQUIRE( test_file_1M(digest_No5, FUNC) );
  }

} // namespace test_sha224

namespace test_sha256 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::SHA256;

  TEST_CASE("Compute SHA-256 message digests.", "[sha256]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad};
    REQUIRE( test_digest("abc",
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55};
    REQUIRE( test_digest("",
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8, 0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39, 0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67, 0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1};
    REQUIRE( test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0xcf, 0x5b, 0x16, 0xa7, 0x78, 0xaf, 0x83, 0x80, 0x03, 0x6c, 0xe5, 0x9e, 0x7b, 0x04, 0x92, 0x37, 0x0b, 0x24, 0x9b, 0x11, 0xe8, 0xf0, 0x7a, 0x51, 0xaf, 0xac, 0x45, 0x03, 0x7a, 0xfe, 0xe9, 0xd1};
    REQUIRE( test_digest("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0xcd, 0xc7, 0x6e, 0x5c, 0x99, 0x14, 0xfb, 0x92, 0x81, 0xa1, 0xc7, 0xe2, 0x84, 0xd7, 0x3e, 0x67, 0xf1, 0x80, 0x9a, 0x48, 0xa4, 0x97, 0x20, 0x0e, 0x04, 0x6d, 0x39, 0xcc, 0xc7, 0x11, 0x2c, 0xd0};
    REQUIRE( test_digest(makeMessage1M(),
                         digest_No5, FUNC) );

#ifdef HAVE_MESSAGE_XL
    printf("Make message XL...\n"); fflush(stdout);
    const std::string message_No6 = makeMessageXL();
    const cs::Buffer digest_No6{0x50, 0xe7, 0x2a, 0x0e, 0x26, 0x44, 0x2f, 0xe2, 0x55, 0x2d, 0xc3, 0x93, 0x8a, 0xc5, 0x86, 0x58, 0x22, 0x8c, 0x0c, 0xbf, 0xb1, 0xd2, 0xca, 0x87, 0x2a, 0xe4, 0x35, 0x26, 0x6f, 0xcd, 0x05, 0x5e};
    printf("Hash message XL...\n"); fflush(stdout);
    REQUIRE( test_digest(message_No6,
                         digest_No6, FUNC) );
#endif

    REQUIRE( test_cavp("./testdata/SHA256ShortMsg.rsp", FUNC) );
    REQUIRE( test_cavp("./testdata/SHA256LongMsg.rsp", FUNC) );
    REQUIRE( test_file_1M(digest_No5, FUNC) );
  }

} // namespace test_sha256

namespace test_sha384 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::SHA384;

  TEST_CASE("Compute SHA-384 message digests.", "[sha384]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0xcb, 0x00, 0x75, 0x3f, 0x45, 0xa3, 0x5e, 0x8b, 0xb5, 0xa0, 0x3d, 0x69, 0x9a, 0xc6, 0x50, 0x07, 0x27, 0x2c, 0x32, 0xab, 0x0e, 0xde, 0xd1, 0x63, 0x1a, 0x8b, 0x60, 0x5a, 0x43, 0xff, 0x5b, 0xed, 0x80, 0x86, 0x07, 0x2b, 0xa1, 0xe7, 0xcc, 0x23, 0x58, 0xba, 0xec, 0xa1, 0x34, 0xc8, 0x25, 0xa7};
    REQUIRE( test_digest("abc",
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0x38, 0xb0, 0x60, 0xa7, 0x51, 0xac, 0x96, 0x38, 0x4c, 0xd9, 0x32, 0x7e, 0xb1, 0xb1, 0xe3, 0x6a, 0x21, 0xfd, 0xb7, 0x11, 0x14, 0xbe, 0x07, 0x43, 0x4c, 0x0c, 0xc7, 0xbf, 0x63, 0xf6, 0xe1, 0xda, 0x27, 0x4e, 0xde, 0xbf, 0xe7, 0x6f, 0x65, 0xfb, 0xd5, 0x1a, 0xd2, 0xf1, 0x48, 0x98, 0xb9, 0x5b};
    REQUIRE( test_digest("",
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x33, 0x91, 0xfd, 0xdd, 0xfc, 0x8d, 0xc7, 0x39, 0x37, 0x07, 0xa6, 0x5b, 0x1b, 0x47, 0x09, 0x39, 0x7c, 0xf8, 0xb1, 0xd1, 0x62, 0xaf, 0x05, 0xab, 0xfe, 0x8f, 0x45, 0x0d, 0xe5, 0xf3, 0x6b, 0xc6, 0xb0, 0x45, 0x5a, 0x85, 0x20, 0xbc, 0x4e, 0x6f, 0x5f, 0xe9, 0x5b, 0x1f, 0xe3, 0xc8, 0x45, 0x2b};
    REQUIRE( test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0x09, 0x33, 0x0c, 0x33, 0xf7, 0x11, 0x47, 0xe8, 0x3d, 0x19, 0x2f, 0xc7, 0x82, 0xcd, 0x1b, 0x47, 0x53, 0x11, 0x1b, 0x17, 0x3b, 0x3b, 0x05, 0xd2, 0x2f, 0xa0, 0x80, 0x86, 0xe3, 0xb0, 0xf7, 0x12, 0xfc, 0xc7, 0xc7, 0x1a, 0x55, 0x7e, 0x2d, 0xb9, 0x66, 0xc3, 0xe9, 0xfa, 0x91, 0x74, 0x60, 0x39};
    REQUIRE( test_digest("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0x9d, 0x0e, 0x18, 0x09, 0x71, 0x64, 0x74, 0xcb, 0x08, 0x6e, 0x83, 0x4e, 0x31, 0x0a, 0x4a, 0x1c, 0xed, 0x14, 0x9e, 0x9c, 0x00, 0xf2, 0x48, 0x52, 0x79, 0x72, 0xce, 0xc5, 0x70, 0x4c, 0x2a, 0x5b, 0x07, 0xb8, 0xb3, 0xdc, 0x38, 0xec, 0xc4, 0xeb, 0xae, 0x97, 0xdd, 0xd8, 0x7f, 0x3d, 0x89, 0x85};
    REQUIRE( test_digest(makeMessage1M(),
                         digest_No5, FUNC) );

#ifdef HAVE_MESSAGE_XL
    printf("Make message XL...\n"); fflush(stdout);
    const std::string message_No6 = makeMessageXL();
    const cs::Buffer digest_No6{0x54, 0x41, 0x23, 0x5c, 0xc0, 0x23, 0x53, 0x41, 0xed, 0x80, 0x6a, 0x64, 0xfb, 0x35, 0x47, 0x42, 0xb5, 0xe5, 0xc0, 0x2a, 0x3c, 0x5c, 0xb7, 0x1b, 0x5f, 0x63, 0xfb, 0x79, 0x34, 0x58, 0xd8, 0xfd, 0xae, 0x59, 0x9c, 0x8c, 0xd8, 0x88, 0x49, 0x43, 0xc0, 0x4f, 0x11, 0xb3, 0x1b, 0x89, 0xf0, 0x23};
    printf("Hash message XL...\n"); fflush(stdout);
    REQUIRE( test_digest(message_No6,
                         digest_No6, FUNC) );
#endif

    REQUIRE( test_cavp("./testdata/SHA384ShortMsg.rsp", FUNC) );
    REQUIRE( test_cavp("./testdata/SHA384LongMsg.rsp", FUNC) );
    REQUIRE( test_file_1M(digest_No5, FUNC) );
  }

} // namespace test_sha384

namespace sha512 {

  using namespace test_util;

  constexpr cs::Hash::Function FUNC = cs::Hash::SHA512;

  TEST_CASE("Compute SHA-512 message digests.", "[sha512]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::Buffer digest_No1{0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba, 0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31, 0x12, 0xe6, 0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2, 0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a, 0x21, 0x92, 0x99, 0x2a, 0x27, 0x4f, 0xc1, 0xa8, 0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd, 0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 0xe8, 0x0e, 0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f};
    REQUIRE( test_digest("abc",
                         digest_No1, FUNC) );

    const cs::Buffer digest_No2{0xcf, 0x83, 0xe1, 0x35, 0x7e, 0xef, 0xb8, 0xbd, 0xf1, 0x54, 0x28, 0x50, 0xd6, 0x6d, 0x80, 0x07, 0xd6, 0x20, 0xe4, 0x05, 0x0b, 0x57, 0x15, 0xdc, 0x83, 0xf4, 0xa9, 0x21, 0xd3, 0x6c, 0xe9, 0xce, 0x47, 0xd0, 0xd1, 0x3c, 0x5d, 0x85, 0xf2, 0xb0, 0xff, 0x83, 0x18, 0xd2, 0x87, 0x7e, 0xec, 0x2f, 0x63, 0xb9, 0x31, 0xbd, 0x47, 0x41, 0x7a, 0x81, 0xa5, 0x38, 0x32, 0x7a, 0xf9, 0x27, 0xda, 0x3e};
    REQUIRE( test_digest("",
                         digest_No2, FUNC) );

    const cs::Buffer digest_No3{0x20, 0x4a, 0x8f, 0xc6, 0xdd, 0xa8, 0x2f, 0x0a, 0x0c, 0xed, 0x7b, 0xeb, 0x8e, 0x08, 0xa4, 0x16, 0x57, 0xc1, 0x6e, 0xf4, 0x68, 0xb2, 0x28, 0xa8, 0x27, 0x9b, 0xe3, 0x31, 0xa7, 0x03, 0xc3, 0x35, 0x96, 0xfd, 0x15, 0xc1, 0x3b, 0x1b, 0x07, 0xf9, 0xaa, 0x1d, 0x3b, 0xea, 0x57, 0x78, 0x9c, 0xa0, 0x31, 0xad, 0x85, 0xc7, 0xa7, 0x1d, 0xd7, 0x03, 0x54, 0xec, 0x63, 0x12, 0x38, 0xca, 0x34, 0x45};
    REQUIRE( test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                         digest_No3, FUNC) );

    const cs::Buffer digest_No4{0x8e, 0x95, 0x9b, 0x75, 0xda, 0xe3, 0x13, 0xda, 0x8c, 0xf4, 0xf7, 0x28, 0x14, 0xfc, 0x14, 0x3f, 0x8f, 0x77, 0x79, 0xc6, 0xeb, 0x9f, 0x7f, 0xa1, 0x72, 0x99, 0xae, 0xad, 0xb6, 0x88, 0x90, 0x18, 0x50, 0x1d, 0x28, 0x9e, 0x49, 0x00, 0xf7, 0xe4, 0x33, 0x1b, 0x99, 0xde, 0xc4, 0xb5, 0x43, 0x3a, 0xc7, 0xd3, 0x29, 0xee, 0xb6, 0xdd, 0x26, 0x54, 0x5e, 0x96, 0xe5, 0x5b, 0x87, 0x4b, 0xe9, 0x09};
    REQUIRE( test_digest("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                         digest_No4, FUNC) );

    const cs::Buffer digest_No5{0xe7, 0x18, 0x48, 0x3d, 0x0c, 0xe7, 0x69, 0x64, 0x4e, 0x2e, 0x42, 0xc7, 0xbc, 0x15, 0xb4, 0x63, 0x8e, 0x1f, 0x98, 0xb1, 0x3b, 0x20, 0x44, 0x28, 0x56, 0x32, 0xa8, 0x03, 0xaf, 0xa9, 0x73, 0xeb, 0xde, 0x0f, 0xf2, 0x44, 0x87, 0x7e, 0xa6, 0x0a, 0x4c, 0xb0, 0x43, 0x2c, 0xe5, 0x77, 0xc3, 0x1b, 0xeb, 0x00, 0x9c, 0x5c, 0x2c, 0x49, 0xaa, 0x2e, 0x4e, 0xad, 0xb2, 0x17, 0xad, 0x8c, 0xc0, 0x9b};
    REQUIRE( test_digest(makeMessage1M(),
                         digest_No5, FUNC) );

#ifdef HAVE_MESSAGE_XL
    printf("Make message XL...\n"); fflush(stdout);
    const std::string message_No6 = makeMessageXL();
    const cs::Buffer digest_No6{0xb4, 0x7c, 0x93, 0x34, 0x21, 0xea, 0x2d, 0xb1, 0x49, 0xad, 0x6e, 0x10, 0xfc, 0xe6, 0xc7, 0xf9, 0x3d, 0x07, 0x52, 0x38, 0x01, 0x80, 0xff, 0xd7, 0xf4, 0x62, 0x9a, 0x71, 0x21, 0x34, 0x83, 0x1d, 0x77, 0xbe, 0x60, 0x91, 0xb8, 0x19, 0xed, 0x35, 0x2c, 0x29, 0x67, 0xa2, 0xe2, 0xd4, 0xfa, 0x50, 0x50, 0x72, 0x3c, 0x96, 0x30, 0x69, 0x1f, 0x1a, 0x05, 0xa7, 0x28, 0x1d, 0xbe, 0x6c, 0x10, 0x86};
    printf("Hash message XL...\n"); fflush(stdout);
    REQUIRE( test_digest(message_No6,
                         digest_No6, FUNC) );
#endif

    REQUIRE( test_cavp("./testdata/SHA512ShortMsg.rsp", FUNC) );
    REQUIRE( test_cavp("./testdata/SHA512LongMsg.rsp", FUNC) );
    REQUIRE( test_file_1M(digest_No5, FUNC) );
  }

} // namespace sha512

namespace siphash {

  // https://github.com/veorq/SipHash/blob/master/vectors.h

  using TestVectors64 = std::vector<cs::ByteArray<8>>;

  TestVectors64 make_sip_2_4_64()
  {
    TestVectors64 result;

    result.push_back({0x31, 0x0e, 0x0e, 0xdd, 0x47, 0xdb, 0x6f, 0x72});
    result.push_back({0xfd, 0x67, 0xdc, 0x93, 0xc5, 0x39, 0xf8, 0x74});
    result.push_back({0x5a, 0x4f, 0xa9, 0xd9, 0x09, 0x80, 0x6c, 0x0d});
    result.push_back({0x2d, 0x7e, 0xfb, 0xd7, 0x96, 0x66, 0x67, 0x85});
    result.push_back({0xb7, 0x87, 0x71, 0x27, 0xe0, 0x94, 0x27, 0xcf});
    result.push_back({0x8d, 0xa6, 0x99, 0xcd, 0x64, 0x55, 0x76, 0x18});
    result.push_back({0xce, 0xe3, 0xfe, 0x58, 0x6e, 0x46, 0xc9, 0xcb});
    result.push_back({0x37, 0xd1, 0x01, 0x8b, 0xf5, 0x00, 0x02, 0xab});
    result.push_back({0x62, 0x24, 0x93, 0x9a, 0x79, 0xf5, 0xf5, 0x93});
    result.push_back({0xb0, 0xe4, 0xa9, 0x0b, 0xdf, 0x82, 0x00, 0x9e});
    result.push_back({0xf3, 0xb9, 0xdd, 0x94, 0xc5, 0xbb, 0x5d, 0x7a});
    result.push_back({0xa7, 0xad, 0x6b, 0x22, 0x46, 0x2f, 0xb3, 0xf4});
    result.push_back({0xfb, 0xe5, 0x0e, 0x86, 0xbc, 0x8f, 0x1e, 0x75});
    result.push_back({0x90, 0x3d, 0x84, 0xc0, 0x27, 0x56, 0xea, 0x14});
    result.push_back({0xee, 0xf2, 0x7a, 0x8e, 0x90, 0xca, 0x23, 0xf7});
    result.push_back({0xe5, 0x45, 0xbe, 0x49, 0x61, 0xca, 0x29, 0xa1});
    result.push_back({0xdb, 0x9b, 0xc2, 0x57, 0x7f, 0xcc, 0x2a, 0x3f});
    result.push_back({0x94, 0x47, 0xbe, 0x2c, 0xf5, 0xe9, 0x9a, 0x69});
    result.push_back({0x9c, 0xd3, 0x8d, 0x96, 0xf0, 0xb3, 0xc1, 0x4b});
    result.push_back({0xbd, 0x61, 0x79, 0xa7, 0x1d, 0xc9, 0x6d, 0xbb});
    result.push_back({0x98, 0xee, 0xa2, 0x1a, 0xf2, 0x5c, 0xd6, 0xbe});
    result.push_back({0xc7, 0x67, 0x3b, 0x2e, 0xb0, 0xcb, 0xf2, 0xd0});
    result.push_back({0x88, 0x3e, 0xa3, 0xe3, 0x95, 0x67, 0x53, 0x93});
    result.push_back({0xc8, 0xce, 0x5c, 0xcd, 0x8c, 0x03, 0x0c, 0xa8});
    result.push_back({0x94, 0xaf, 0x49, 0xf6, 0xc6, 0x50, 0xad, 0xb8});
    result.push_back({0xea, 0xb8, 0x85, 0x8a, 0xde, 0x92, 0xe1, 0xbc});
    result.push_back({0xf3, 0x15, 0xbb, 0x5b, 0xb8, 0x35, 0xd8, 0x17});
    result.push_back({0xad, 0xcf, 0x6b, 0x07, 0x63, 0x61, 0x2e, 0x2f});
    result.push_back({0xa5, 0xc9, 0x1d, 0xa7, 0xac, 0xaa, 0x4d, 0xde});
    result.push_back({0x71, 0x65, 0x95, 0x87, 0x66, 0x50, 0xa2, 0xa6});
    result.push_back({0x28, 0xef, 0x49, 0x5c, 0x53, 0xa3, 0x87, 0xad});
    result.push_back({0x42, 0xc3, 0x41, 0xd8, 0xfa, 0x92, 0xd8, 0x32});
    result.push_back({0xce, 0x7c, 0xf2, 0x72, 0x2f, 0x51, 0x27, 0x71});
    result.push_back({0xe3, 0x78, 0x59, 0xf9, 0x46, 0x23, 0xf3, 0xa7});
    result.push_back({0x38, 0x12, 0x05, 0xbb, 0x1a, 0xb0, 0xe0, 0x12});
    result.push_back({0xae, 0x97, 0xa1, 0x0f, 0xd4, 0x34, 0xe0, 0x15});
    result.push_back({0xb4, 0xa3, 0x15, 0x08, 0xbe, 0xff, 0x4d, 0x31});
    result.push_back({0x81, 0x39, 0x62, 0x29, 0xf0, 0x90, 0x79, 0x02});
    result.push_back({0x4d, 0x0c, 0xf4, 0x9e, 0xe5, 0xd4, 0xdc, 0xca});
    result.push_back({0x5c, 0x73, 0x33, 0x6a, 0x76, 0xd8, 0xbf, 0x9a});
    result.push_back({0xd0, 0xa7, 0x04, 0x53, 0x6b, 0xa9, 0x3e, 0x0e});
    result.push_back({0x92, 0x59, 0x58, 0xfc, 0xd6, 0x42, 0x0c, 0xad});
    result.push_back({0xa9, 0x15, 0xc2, 0x9b, 0xc8, 0x06, 0x73, 0x18});
    result.push_back({0x95, 0x2b, 0x79, 0xf3, 0xbc, 0x0a, 0xa6, 0xd4});
    result.push_back({0xf2, 0x1d, 0xf2, 0xe4, 0x1d, 0x45, 0x35, 0xf9});
    result.push_back({0x87, 0x57, 0x75, 0x19, 0x04, 0x8f, 0x53, 0xa9});
    result.push_back({0x10, 0xa5, 0x6c, 0xf5, 0xdf, 0xcd, 0x9a, 0xdb});
    result.push_back({0xeb, 0x75, 0x09, 0x5c, 0xcd, 0x98, 0x6c, 0xd0});
    result.push_back({0x51, 0xa9, 0xcb, 0x9e, 0xcb, 0xa3, 0x12, 0xe6});
    result.push_back({0x96, 0xaf, 0xad, 0xfc, 0x2c, 0xe6, 0x66, 0xc7});
    result.push_back({0x72, 0xfe, 0x52, 0x97, 0x5a, 0x43, 0x64, 0xee});
    result.push_back({0x5a, 0x16, 0x45, 0xb2, 0x76, 0xd5, 0x92, 0xa1});
    result.push_back({0xb2, 0x74, 0xcb, 0x8e, 0xbf, 0x87, 0x87, 0x0a});
    result.push_back({0x6f, 0x9b, 0xb4, 0x20, 0x3d, 0xe7, 0xb3, 0x81});
    result.push_back({0xea, 0xec, 0xb2, 0xa3, 0x0b, 0x22, 0xa8, 0x7f});
    result.push_back({0x99, 0x24, 0xa4, 0x3c, 0xc1, 0x31, 0x57, 0x24});
    result.push_back({0xbd, 0x83, 0x8d, 0x3a, 0xaf, 0xbf, 0x8d, 0xb7});
    result.push_back({0x0b, 0x1a, 0x2a, 0x32, 0x65, 0xd5, 0x1a, 0xea});
    result.push_back({0x13, 0x50, 0x79, 0xa3, 0x23, 0x1c, 0xe6, 0x60});
    result.push_back({0x93, 0x2b, 0x28, 0x46, 0xe4, 0xd7, 0x06, 0x66});
    result.push_back({0xe1, 0x91, 0x5f, 0x5c, 0xb1, 0xec, 0xa4, 0x6c});
    result.push_back({0xf3, 0x25, 0x96, 0x5c, 0xa1, 0x6d, 0x62, 0x9f});
    result.push_back({0x57, 0x5f, 0xf2, 0x8e, 0x60, 0x38, 0x1b, 0xe5});
    result.push_back({0x72, 0x45, 0x06, 0xeb, 0x4c, 0x32, 0x8a, 0x95});

    return result;
  }

  using TestVectors128 = std::vector<cs::ByteArray<16>>;

  TestVectors128 make_sip_2_4_128()
  {
    TestVectors128 result;

    result.push_back({0xa3, 0x81, 0x7f, 0x04, 0xba, 0x25, 0xa8, 0xe6, 0x6d, 0xf6, 0x72, 0x14, 0xc7, 0x55, 0x02, 0x93});
    result.push_back({0xda, 0x87, 0xc1, 0xd8, 0x6b, 0x99, 0xaf, 0x44, 0x34, 0x76, 0x59, 0x11, 0x9b, 0x22, 0xfc, 0x45});
    result.push_back({0x81, 0x77, 0x22, 0x8d, 0xa4, 0xa4, 0x5d, 0xc7, 0xfc, 0xa3, 0x8b, 0xde, 0xf6, 0x0a, 0xff, 0xe4});
    result.push_back({0x9c, 0x70, 0xb6, 0x0c, 0x52, 0x67, 0xa9, 0x4e, 0x5f, 0x33, 0xb6, 0xb0, 0x29, 0x85, 0xed, 0x51});
    result.push_back({0xf8, 0x81, 0x64, 0xc1, 0x2d, 0x9c, 0x8f, 0xaf, 0x7d, 0x0f, 0x6e, 0x7c, 0x7b, 0xcd, 0x55, 0x79});
    result.push_back({0x13, 0x68, 0x87, 0x59, 0x80, 0x77, 0x6f, 0x88, 0x54, 0x52, 0x7a, 0x07, 0x69, 0x0e, 0x96, 0x27});
    result.push_back({0x14, 0xee, 0xca, 0x33, 0x8b, 0x20, 0x86, 0x13, 0x48, 0x5e, 0xa0, 0x30, 0x8f, 0xd7, 0xa1, 0x5e});
    result.push_back({0xa1, 0xf1, 0xeb, 0xbe, 0xd8, 0xdb, 0xc1, 0x53, 0xc0, 0xb8, 0x4a, 0xa6, 0x1f, 0xf0, 0x82, 0x39});
    result.push_back({0x3b, 0x62, 0xa9, 0xba, 0x62, 0x58, 0xf5, 0x61, 0x0f, 0x83, 0xe2, 0x64, 0xf3, 0x14, 0x97, 0xb4});
    result.push_back({0x26, 0x44, 0x99, 0x06, 0x0a, 0xd9, 0xba, 0xab, 0xc4, 0x7f, 0x8b, 0x02, 0xbb, 0x6d, 0x71, 0xed});
    result.push_back({0x00, 0x11, 0x0d, 0xc3, 0x78, 0x14, 0x69, 0x56, 0xc9, 0x54, 0x47, 0xd3, 0xf3, 0xd0, 0xfb, 0xba});
    result.push_back({0x01, 0x51, 0xc5, 0x68, 0x38, 0x6b, 0x66, 0x77, 0xa2, 0xb4, 0xdc, 0x6f, 0x81, 0xe5, 0xdc, 0x18});
    result.push_back({0xd6, 0x26, 0xb2, 0x66, 0x90, 0x5e, 0xf3, 0x58, 0x82, 0x63, 0x4d, 0xf6, 0x85, 0x32, 0xc1, 0x25});
    result.push_back({0x98, 0x69, 0xe2, 0x47, 0xe9, 0xc0, 0x8b, 0x10, 0xd0, 0x29, 0x93, 0x4f, 0xc4, 0xb9, 0x52, 0xf7});
    result.push_back({0x31, 0xfc, 0xef, 0xac, 0x66, 0xd7, 0xde, 0x9c, 0x7e, 0xc7, 0x48, 0x5f, 0xe4, 0x49, 0x49, 0x02});
    result.push_back({0x54, 0x93, 0xe9, 0x99, 0x33, 0xb0, 0xa8, 0x11, 0x7e, 0x08, 0xec, 0x0f, 0x97, 0xcf, 0xc3, 0xd9});
    result.push_back({0x6e, 0xe2, 0xa4, 0xca, 0x67, 0xb0, 0x54, 0xbb, 0xfd, 0x33, 0x15, 0xbf, 0x85, 0x23, 0x05, 0x77});
    result.push_back({0x47, 0x3d, 0x06, 0xe8, 0x73, 0x8d, 0xb8, 0x98, 0x54, 0xc0, 0x66, 0xc4, 0x7a, 0xe4, 0x77, 0x40});
    result.push_back({0xa4, 0x26, 0xe5, 0xe4, 0x23, 0xbf, 0x48, 0x85, 0x29, 0x4d, 0xa4, 0x81, 0xfe, 0xae, 0xf7, 0x23});
    result.push_back({0x78, 0x01, 0x77, 0x31, 0xcf, 0x65, 0xfa, 0xb0, 0x74, 0xd5, 0x20, 0x89, 0x52, 0x51, 0x2e, 0xb1});
    result.push_back({0x9e, 0x25, 0xfc, 0x83, 0x3f, 0x22, 0x90, 0x73, 0x3e, 0x93, 0x44, 0xa5, 0xe8, 0x38, 0x39, 0xeb});
    result.push_back({0x56, 0x8e, 0x49, 0x5a, 0xbe, 0x52, 0x5a, 0x21, 0x8a, 0x22, 0x14, 0xcd, 0x3e, 0x07, 0x1d, 0x12});
    result.push_back({0x4a, 0x29, 0xb5, 0x45, 0x52, 0xd1, 0x6b, 0x9a, 0x46, 0x9c, 0x10, 0x52, 0x8e, 0xff, 0x0a, 0xae});
    result.push_back({0xc9, 0xd1, 0x84, 0xdd, 0xd5, 0xa9, 0xf5, 0xe0, 0xcf, 0x8c, 0xe2, 0x9a, 0x9a, 0xbf, 0x69, 0x1c});
    result.push_back({0x2d, 0xb4, 0x79, 0xae, 0x78, 0xbd, 0x50, 0xd8, 0x88, 0x2a, 0x8a, 0x17, 0x8a, 0x61, 0x32, 0xad});
    result.push_back({0x8e, 0xce, 0x5f, 0x04, 0x2d, 0x5e, 0x44, 0x7b, 0x50, 0x51, 0xb9, 0xea, 0xcb, 0x8d, 0x8f, 0x6f});
    result.push_back({0x9c, 0x0b, 0x53, 0xb4, 0xb3, 0xc3, 0x07, 0xe8, 0x7e, 0xae, 0xe0, 0x86, 0x78, 0x14, 0x1f, 0x66});
    result.push_back({0xab, 0xf2, 0x48, 0xaf, 0x69, 0xa6, 0xea, 0xe4, 0xbf, 0xd3, 0xeb, 0x2f, 0x12, 0x9e, 0xeb, 0x94});
    result.push_back({0x06, 0x64, 0xda, 0x16, 0x68, 0x57, 0x4b, 0x88, 0xb9, 0x35, 0xf3, 0x02, 0x73, 0x58, 0xae, 0xf4});
    result.push_back({0xaa, 0x4b, 0x9d, 0xc4, 0xbf, 0x33, 0x7d, 0xe9, 0x0c, 0xd4, 0xfd, 0x3c, 0x46, 0x7c, 0x6a, 0xb7});
    result.push_back({0xea, 0x5c, 0x7f, 0x47, 0x1f, 0xaf, 0x6b, 0xde, 0x2b, 0x1a, 0xd7, 0xd4, 0x68, 0x6d, 0x22, 0x87});
    result.push_back({0x29, 0x39, 0xb0, 0x18, 0x32, 0x23, 0xfa, 0xfc, 0x17, 0x23, 0xde, 0x4f, 0x52, 0xc4, 0x3d, 0x35});
    result.push_back({0x7c, 0x39, 0x56, 0xca, 0x5e, 0xea, 0xfc, 0x3e, 0x36, 0x3e, 0x9d, 0x55, 0x65, 0x46, 0xeb, 0x68});
    result.push_back({0x77, 0xc6, 0x07, 0x71, 0x46, 0xf0, 0x1c, 0x32, 0xb6, 0xb6, 0x9d, 0x5f, 0x4e, 0xa9, 0xff, 0xcf});
    result.push_back({0x37, 0xa6, 0x98, 0x6c, 0xb8, 0x84, 0x7e, 0xdf, 0x09, 0x25, 0xf0, 0xf1, 0x30, 0x9b, 0x54, 0xde});
    result.push_back({0xa7, 0x05, 0xf0, 0xe6, 0x9d, 0xa9, 0xa8, 0xf9, 0x07, 0x24, 0x1a, 0x2e, 0x92, 0x3c, 0x8c, 0xc8});
    result.push_back({0x3d, 0xc4, 0x7d, 0x1f, 0x29, 0xc4, 0x48, 0x46, 0x1e, 0x9e, 0x76, 0xed, 0x90, 0x4f, 0x67, 0x11});
    result.push_back({0x0d, 0x62, 0xbf, 0x01, 0xe6, 0xfc, 0x0e, 0x1a, 0x0d, 0x3c, 0x47, 0x51, 0xc5, 0xd3, 0x69, 0x2b});
    result.push_back({0x8c, 0x03, 0x46, 0x8b, 0xca, 0x7c, 0x66, 0x9e, 0xe4, 0xfd, 0x5e, 0x08, 0x4b, 0xbe, 0xe7, 0xb5});
    result.push_back({0x52, 0x8a, 0x5b, 0xb9, 0x3b, 0xaf, 0x2c, 0x9c, 0x44, 0x73, 0xcc, 0xe5, 0xd0, 0xd2, 0x2b, 0xd9});
    result.push_back({0xdf, 0x6a, 0x30, 0x1e, 0x95, 0xc9, 0x5d, 0xad, 0x97, 0xae, 0x0c, 0xc8, 0xc6, 0x91, 0x3b, 0xd8});
    result.push_back({0x80, 0x11, 0x89, 0x90, 0x2c, 0x85, 0x7f, 0x39, 0xe7, 0x35, 0x91, 0x28, 0x5e, 0x70, 0xb6, 0xdb});
    result.push_back({0xe6, 0x17, 0x34, 0x6a, 0xc9, 0xc2, 0x31, 0xbb, 0x36, 0x50, 0xae, 0x34, 0xcc, 0xca, 0x0c, 0x5b});
    result.push_back({0x27, 0xd9, 0x34, 0x37, 0xef, 0xb7, 0x21, 0xaa, 0x40, 0x18, 0x21, 0xdc, 0xec, 0x5a, 0xdf, 0x89});
    result.push_back({0x89, 0x23, 0x7d, 0x9d, 0xed, 0x9c, 0x5e, 0x78, 0xd8, 0xb1, 0xc9, 0xb1, 0x66, 0xcc, 0x73, 0x42});
    result.push_back({0x4a, 0x6d, 0x80, 0x91, 0xbf, 0x5e, 0x7d, 0x65, 0x11, 0x89, 0xfa, 0x94, 0xa2, 0x50, 0xb1, 0x4c});
    result.push_back({0x0e, 0x33, 0xf9, 0x60, 0x55, 0xe7, 0xae, 0x89, 0x3f, 0xfc, 0x0e, 0x3d, 0xcf, 0x49, 0x29, 0x02});
    result.push_back({0xe6, 0x1c, 0x43, 0x2b, 0x72, 0x0b, 0x19, 0xd1, 0x8e, 0xc8, 0xd8, 0x4b, 0xdc, 0x63, 0x15, 0x1b});
    result.push_back({0xf7, 0xe5, 0xae, 0xf5, 0x49, 0xf7, 0x82, 0xcf, 0x37, 0x90, 0x55, 0xa6, 0x08, 0x26, 0x9b, 0x16});
    result.push_back({0x43, 0x8d, 0x03, 0x0f, 0xd0, 0xb7, 0xa5, 0x4f, 0xa8, 0x37, 0xf2, 0xad, 0x20, 0x1a, 0x64, 0x03});
    result.push_back({0xa5, 0x90, 0xd3, 0xee, 0x4f, 0xbf, 0x04, 0xe3, 0x24, 0x7e, 0x0d, 0x27, 0xf2, 0x86, 0x42, 0x3f});
    result.push_back({0x5f, 0xe2, 0xc1, 0xa1, 0x72, 0xfe, 0x93, 0xc4, 0xb1, 0x5c, 0xd3, 0x7c, 0xae, 0xf9, 0xf5, 0x38});
    result.push_back({0x2c, 0x97, 0x32, 0x5c, 0xbd, 0x06, 0xb3, 0x6e, 0xb2, 0x13, 0x3d, 0xd0, 0x8b, 0x3a, 0x01, 0x7c});
    result.push_back({0x92, 0xc8, 0x14, 0x22, 0x7a, 0x6b, 0xca, 0x94, 0x9f, 0xf0, 0x65, 0x9f, 0x00, 0x2a, 0xd3, 0x9e});
    result.push_back({0xdc, 0xe8, 0x50, 0x11, 0x0b, 0xd8, 0x32, 0x8c, 0xfb, 0xd5, 0x08, 0x41, 0xd6, 0x91, 0x1d, 0x87});
    result.push_back({0x67, 0xf1, 0x49, 0x84, 0xc7, 0xda, 0x79, 0x12, 0x48, 0xe3, 0x2b, 0xb5, 0x92, 0x25, 0x83, 0xda});
    result.push_back({0x19, 0x38, 0xf2, 0xcf, 0x72, 0xd5, 0x4e, 0xe9, 0x7e, 0x94, 0x16, 0x6f, 0xa9, 0x1d, 0x2a, 0x36});
    result.push_back({0x74, 0x48, 0x1e, 0x96, 0x46, 0xed, 0x49, 0xfe, 0x0f, 0x62, 0x24, 0x30, 0x16, 0x04, 0x69, 0x8e});
    result.push_back({0x57, 0xfc, 0xa5, 0xde, 0x98, 0xa9, 0xd6, 0xd8, 0x00, 0x64, 0x38, 0xd0, 0x58, 0x3d, 0x8a, 0x1d});
    result.push_back({0x9f, 0xec, 0xde, 0x1c, 0xef, 0xdc, 0x1c, 0xbe, 0xd4, 0x76, 0x36, 0x74, 0xd9, 0x57, 0x53, 0x59});
    result.push_back({0xe3, 0x04, 0x0c, 0x00, 0xeb, 0x28, 0xf1, 0x53, 0x66, 0xca, 0x73, 0xcb, 0xd8, 0x72, 0xe7, 0x40});
    result.push_back({0x76, 0x97, 0x00, 0x9a, 0x6a, 0x83, 0x1d, 0xfe, 0xcc, 0xa9, 0x1c, 0x59, 0x93, 0x67, 0x0f, 0x7a});
    result.push_back({0x58, 0x53, 0x54, 0x23, 0x21, 0xf5, 0x67, 0xa0, 0x05, 0xd5, 0x47, 0xa4, 0xf0, 0x47, 0x59, 0xbd});
    result.push_back({0x51, 0x50, 0xd1, 0x77, 0x2f, 0x50, 0x83, 0x4a, 0x50, 0x3e, 0x06, 0x9a, 0x97, 0x3f, 0xbd, 0x7c});

    return result;
  }

  TEST_CASE("Compute SipHash-2-4-64 message digests.", "[siphash64]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    cs::ByteArray<64> input;
    std::iota(input.begin(), input.end(), 0);

    cs::ByteArray<16> key;
    std::iota(key.begin(), key.end(), 0);

    const cs::HashPtr siphash = cs::Hash::make(cs::Hash::SipHash_2_4_64);
    siphash->setKey(key.data());

    const TestVectors64 vectors = make_sip_2_4_64();
    for(cs::size_t i = 0; i < vectors.size(); i++) {
      siphash->update(input.data(), i);
      const cs::Buffer result = siphash->digest();

      REQUIRE( std::equal(result.begin(), result.end(), vectors[i].begin()) );
    }
  }

  TEST_CASE("Compute SipHash-2-4-128 message digests.", "[siphash128]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    cs::ByteArray<64> input;
    std::iota(input.begin(), input.end(), 0);

    cs::ByteArray<16> key;
    std::iota(key.begin(), key.end(), 0);

    const cs::HashPtr siphash = cs::Hash::make(cs::Hash::SipHash_2_4_128);
    siphash->setKey(key.data());

    const TestVectors128 vectors = make_sip_2_4_128();
    for(cs::size_t i = 0; i < vectors.size(); i++) {
      siphash->update(input.data(), i);
      const cs::Buffer result = siphash->digest();

      REQUIRE( std::equal(result.begin(), result.end(), vectors[i].begin()) );
    }
  }

} // namespace siphash
