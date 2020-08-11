#include <cstdio>
#include <cstdlib>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <csUtil/csFlags.h>

template<typename EnumT>
void print(const csFlags<EnumT>& flags)
{
  const typename csFlags<EnumT>::value_type v = flags;
  printf("value = 0x%X\n", v);
}

namespace flags {

  enum Bit : unsigned {
    NoBits  =  0,
    Bit0    =  1,
    Bit1    =  2,
    Bit2    =  4,
    Bit3    =  8,
    AllBits = 15
  };

} // namespace flags

CS_ENABLE_FLAGS(flags::Bit)

namespace flags {

  using Bits = csFlags<Bit>;

  TEST_CASE("Basic manipulation of csFlags<>", "[flags]") {
    Bits bits;
    print(bits);
    REQUIRE( !bits );

    bits.set(Bit0);
    print(bits);
    REQUIRE(  bits == Bit0 );
    REQUIRE(  bits.testFlags(Bit0) );
    REQUIRE( !bits.testFlags(Bit3) );
    REQUIRE(  bits.testFlags(AllBits) );
    REQUIRE( !bits.testMask(AllBits) );

    bits.set(Bit1 | Bit2 | Bit3);
    print(bits);
    REQUIRE( AllBits == bits );
    REQUIRE( bits.testFlags(Bit3) );
    REQUIRE( bits.testMask(Bit0 | Bit3) );
    REQUIRE( bits.testMask(AllBits) );

    bits.reset(Bit0 | Bit1);
    print(bits);
    REQUIRE(  bits == (Bit2 | Bit3) );
    REQUIRE( !bits.testFlags(Bit0 | Bit1) );
    REQUIRE(  bits.testMask(Bit2 | Bit3) );
  }

} // namespace flags
