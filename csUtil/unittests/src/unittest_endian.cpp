#include <cstdio>
#include <cstdlib>

#include <csUtil/csEndian.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace test_integral {

  uint16_t test_swap(const uint16_t& value)
  {
    return cs::swap<true>(value);
  }

  uint32_t test_swap(const uint32_t& value)
  {
    return cs::swap<true>(value);
  }

  uint64_t test_swap(const uint64_t& value)
  {
    return cs::swap<true>(value);
  }

  TEST_CASE("Swap endianness of integral values.", "[integral]") {
    REQUIRE( uint16_t(0x0201) == test_swap(uint16_t(0x0102)) );

    REQUIRE( uint32_t(0x04030201) == test_swap(uint32_t(0x01020304)) );

    REQUIRE( uint64_t(0x0807060504030201) == test_swap(uint64_t(0x0102030405060708)) );
  }

} // namespace test_integral
