#include <emmintrin.h>

#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/ByteArray.h>
#include <cs/Core/Pointer.h>

namespace util {

  cs::ByteArray<128> data;

} // namespace util

namespace test_pointer {

  template<std::size_t OFFSET>
  inline const void *DAT()
  {
    return util::data.data() + OFFSET;
  }

  using PTR = cs::Pointer;

  TEST_CASE("Check alignment of pointers.", "[is_aligned]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    constexpr std::size_t OFFSET = 32;

    REQUIRE( PTR::isAlignedTo<uint8_t>(DAT<OFFSET + 0>()) );
    REQUIRE( PTR::isAlignedTo<uint8_t>(DAT<OFFSET + 1>()) );

    REQUIRE(  PTR::isAlignedTo<uint16_t>(DAT<OFFSET + 0>()) );
    REQUIRE( !PTR::isAlignedTo<uint16_t>(DAT<OFFSET + 1>()) );
    REQUIRE(  PTR::isAlignedTo<uint16_t>(DAT<OFFSET + 2>()) );

    REQUIRE(  PTR::isAlignedTo<uint32_t>(DAT<OFFSET + 0>()) );
    REQUIRE( !PTR::isAlignedTo<uint32_t>(DAT<OFFSET + 1>()) );
    REQUIRE( !PTR::isAlignedTo<uint32_t>(DAT<OFFSET + 2>()) );
    REQUIRE( !PTR::isAlignedTo<uint32_t>(DAT<OFFSET + 3>()) );
    REQUIRE(  PTR::isAlignedTo<uint32_t>(DAT<OFFSET + 4>()) );

    REQUIRE(  PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 0>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 1>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 2>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 3>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 4>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 5>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 6>()) );
    REQUIRE( !PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 7>()) );
    REQUIRE(  PTR::isAlignedTo<uint64_t>(DAT<OFFSET + 8>()) );

    REQUIRE(  PTR::isAlignedTo<__m128>(DAT<OFFSET +  0>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  1>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  2>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  3>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  4>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  5>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  6>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  7>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  8>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET +  9>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET + 10>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET + 11>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET + 12>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET + 13>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET + 14>()) );
    REQUIRE( !PTR::isAlignedTo<__m128>(DAT<OFFSET + 15>()) );
    REQUIRE(  PTR::isAlignedTo<__m128>(DAT<OFFSET + 16>()) );
  } // TEST_CASE

  TEST_CASE("Align to boundary.", "[align_to]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    constexpr std::size_t OFFSET = 32;

    REQUIRE( PTR::alignTo<uint8_t>(DAT<OFFSET + 0>()) == DAT<OFFSET + 0>() );
    REQUIRE( PTR::alignTo<uint8_t>(DAT<OFFSET + 1>()) == DAT<OFFSET + 1>() );

    REQUIRE( PTR::alignTo<uint16_t>(DAT<OFFSET + 1>()) == DAT<OFFSET + 2>() );
    REQUIRE( PTR::alignTo<uint16_t>(DAT<OFFSET + 2>()) == DAT<OFFSET + 2>() );

    REQUIRE( PTR::alignTo<uint32_t>(DAT<OFFSET + 1>()) == DAT<OFFSET + 4>() );
    REQUIRE( PTR::alignTo<uint32_t>(DAT<OFFSET + 2>()) == DAT<OFFSET + 4>() );
    REQUIRE( PTR::alignTo<uint32_t>(DAT<OFFSET + 3>()) == DAT<OFFSET + 4>() );
    REQUIRE( PTR::alignTo<uint32_t>(DAT<OFFSET + 4>()) == DAT<OFFSET + 4>() );

    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 1>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 2>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 3>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 4>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 5>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 6>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 7>()) == DAT<OFFSET + 8>() );
    REQUIRE( PTR::alignTo<uint64_t>(DAT<OFFSET + 8>()) == DAT<OFFSET + 8>() );

    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  1>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  2>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  3>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  4>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  5>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  6>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  7>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  8>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET +  9>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 10>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 11>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 12>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 13>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 14>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 15>()) == DAT<OFFSET + 16>() );
    REQUIRE( PTR::alignTo<__m128>(DAT<OFFSET + 16>()) == DAT<OFFSET + 16>() );
  } // TEST_CASE

} // namespace test_pointer
