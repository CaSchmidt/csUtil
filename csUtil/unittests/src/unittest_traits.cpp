#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/Convert.h>

namespace test_maxab {

  TEST_CASE("Convert to signed/unsigned.", "[convert]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    REQUIRE( cs::toSigned<int16_t,uint16_t>(0)      == int16_t{0} );
    REQUIRE( cs::toSigned<int16_t,uint16_t>(7)      == int16_t{7} );
    REQUIRE( cs::toSigned<int16_t,uint16_t>(0xFFFF) == int16_t{0x7FFF} );

    REQUIRE( cs::toUnsigned<uint16_t,int>(-1)         == uint16_t{0} );
    REQUIRE( cs::toUnsigned<uint16_t,int>(7)          == uint16_t{7} );
    REQUIRE( cs::toUnsigned<uint16_t,int>(0x7FFFFFFF) == uint16_t{0xFFFF} );
  }

  TEST_CASE("Maximum positive value of types A and B.", "[maxab]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    REQUIRE( cs::maxab_v<int16_t,int8_t>   == int16_t{0x007F} );
    REQUIRE( cs::maxab_v<int16_t,uint8_t>  == int16_t{0x00FF} );
    REQUIRE( cs::maxab_v<int16_t,int16_t>  == int16_t{0x7FFF} );
    REQUIRE( cs::maxab_v<int16_t,uint16_t> == int16_t{0x7FFF} );
    REQUIRE( cs::maxab_v<int16_t,int32_t>  == int16_t{0x7FFF} );
    REQUIRE( cs::maxab_v<int16_t,uint32_t> == int16_t{0x7FFF} );

    REQUIRE( cs::maxab_v<uint16_t,int8_t>   == uint16_t{0x007F} );
    REQUIRE( cs::maxab_v<uint16_t,uint8_t>  == uint16_t{0x00FF} );
    REQUIRE( cs::maxab_v<uint16_t,int16_t>  == uint16_t{0x7FFF} );
    REQUIRE( cs::maxab_v<uint16_t,uint16_t> == uint16_t{0xFFFF} );
    REQUIRE( cs::maxab_v<uint16_t,int32_t>  == uint16_t{0xFFFF} );
    REQUIRE( cs::maxab_v<uint16_t,uint32_t> == uint16_t{0xFFFF} );
  }

} // namespace test_maxab
