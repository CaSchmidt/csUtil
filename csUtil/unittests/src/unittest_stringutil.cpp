#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <catch.hpp>

#include <csUtil/csStringUtil.h>

namespace stringutil {

  const char *STR_abcd = "abcd";
  const char *STR_abc = "abc";
  const char *STR_bcd = "bcd";

  const char *STR_ABCD = "ABCD";
  const char *STR_ABC = "ABC";
  const char *STR_BCD = "BCD";

  TEST_CASE("String ends with pattern.", "[ends]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::endsWith(STR_abcd, STR_bcd) );
    REQUIRE( !cs::endsWith(STR_abcd, STR_BCD) );
    REQUIRE(  cs::endsWith(STR_abcd, STR_BCD, true) );

    REQUIRE(  cs::endsWith(STR_ABCD, STR_BCD) );
    REQUIRE( !cs::endsWith(STR_ABCD, STR_bcd) );
    REQUIRE(  cs::endsWith(STR_ABCD, STR_bcd, true) );

    REQUIRE( !cs::endsWith(STR_bcd, STR_abcd) );
  }

  TEST_CASE("String starts with pattern.", "[starts]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::startsWith(STR_abcd, STR_abc) );
    REQUIRE( !cs::startsWith(STR_abcd, STR_ABC) );
    REQUIRE(  cs::startsWith(STR_abcd, STR_ABC, true) );

    REQUIRE(  cs::startsWith(STR_ABCD, STR_ABC) );
    REQUIRE( !cs::startsWith(STR_ABCD, STR_abc) );
    REQUIRE(  cs::startsWith(STR_ABCD, STR_abc, true) );

    REQUIRE( !cs::startsWith(STR_abc, STR_abcd) );
  }

} // namespace stringutil
