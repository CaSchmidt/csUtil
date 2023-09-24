#include <cstdio>
#include <cstdlib>

#include <catch.hpp>

#include <cs/Text/PrintUtil.h>

namespace test_maxab {

  TEST_CASE("Maximum positive value of types A and B.", "[maxab]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

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
