#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/Flags.h>

template<typename EnumT>
void print(const cs::Flags<EnumT>& flags)
{
  const typename cs::Flags<EnumT>::value_type v = flags;
  std::println("value = 0x{:X}", v);
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

  using Bits = cs::Flags<Bit>;

  TEST_CASE("Basic manipulation of csFlags<>", "[flags]") {
    Bits bits;
    print(bits);
    REQUIRE( !bits );

    bits.set(Bit0);
    print(bits);
    REQUIRE(  bits == Bit0 );
    REQUIRE(  bits.testAny(Bit0) );
    REQUIRE( !bits.testAny(Bit3) );
    REQUIRE(  bits.testAny(AllBits) );
    REQUIRE( !bits.testAll(AllBits) );

    bits.set(Bit1 | Bit2 | Bit3);
    print(bits);
    REQUIRE( AllBits == bits );
    REQUIRE( bits.testAny(Bit3) );
    REQUIRE( bits.testAll(Bit0 | Bit3) );
    REQUIRE( bits.testAll(AllBits) );

    bits.reset(Bit0 | Bit1);
    print(bits);
    REQUIRE(  bits == (Bit2 | Bit3) );
    REQUIRE( !bits.testAny(Bit0 | Bit1) );
    REQUIRE(  bits.testAll(Bit2 | Bit3) );

    bits.toggle(Bit2 | Bit3);
    print(bits);
    REQUIRE( !bits );
  }

} // namespace flags
