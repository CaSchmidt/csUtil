#include <cstdio>
#include <cstdlib>

#include <catch2/catch_test_macros.hpp>

namespace test_integral {

  // Prototypes //////////////////////////////////////////////////////////////

  extern uint16_t test_swap(const uint16_t& value);
  extern uint32_t test_swap(const uint32_t& value);
  extern uint64_t test_swap(const uint64_t& value);

  // Inputs & Results ////////////////////////////////////////////////////////

  const uint16_t     in16{0x0102};
  const uint16_t result16{0x0201};

  const uint32_t     in32{0x01020304};
  const uint32_t result32{0x04030201};

  const uint64_t     in64{0x0102030405060708};
  const uint64_t result64{0x0807060504030201};

  // Output //////////////////////////////////////////////////////////////////

  volatile uint16_t out16;
  volatile uint32_t out32;
  volatile uint64_t out64;

  // Tests ///////////////////////////////////////////////////////////////////

  TEST_CASE("Swap endianness of integral values.", "[integral]") {
    out16 = test_swap(in16);
    REQUIRE( out16 == result16 );

    out32 = test_swap(in32);
    REQUIRE( out32 == result32 );

    out64 = test_swap(in64);
    REQUIRE( out64 == result64 );
  }

} // namespace test_integral
