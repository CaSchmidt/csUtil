#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <csUtil/csStringUtil.h>

namespace stringutil {

  using      String         = cs::String<char>;
  using      StringList     = cs::StringList<char>;
  using ConstStringListIter = cs::ConstStringListIter<char>;

  const char *STR_abcd = "abcd";
  const char *STR_abc  = "abc";
  const char *STR_bcd  = "bcd";

  const char *STR_ABCD = "ABCD";
  const char *STR_ABC  = "ABC";
  const char *STR_BCD  = "BCD";

  TEST_CASE("Case conversion.", "[case]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    {
      String str(STR_ABCD);
      cs::toLower(str);
      REQUIRE( str == STR_abcd );
    }

    {
      String str(STR_abcd);
      cs::toUpper(str);
      REQUIRE( str == STR_ABCD );
    }
  }

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

  TEST_CASE("Remove patterns string.", "[remove]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *STR_input1 = ".ab.cd.";
    const char *STR_input2 = "abcabc";
    const char *STR_input3 = " a b c ";

    {
      String str(STR_input1);
      cs::removeAll(str, '.');
      REQUIRE( str == STR_abcd );
    }

    {
      String str(STR_input2);
      cs::removeAll(str, STR_abc);
      REQUIRE( str.size() == 0 );
    }

    {
      String str(STR_input3);
      cs::removeAll(str.data(), str.data() + str.size(), cs::lambda_is_space<char>());
      cs::shrink(str);
      REQUIRE( str == STR_abc );
    }

    {
      String str(STR_input3);
      cs::removeAll(str, cs::lambda_is_space<char>());
      REQUIRE( str == STR_abc );
    }
  }

  TEST_CASE("Replace part(s) of string.", "[replace]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *STR_input1 = " abc abc ";
    const char *STR_input2 = "abcabc";

    {
      String str(STR_input1);
      cs::replaceAll(str, ' ', ".");
      REQUIRE( str == ".abc.abc." );
    }

    {
      String str(STR_input1);
      cs::replaceAll(str, "abc", "xyz");
      REQUIRE( str == " xyz xyz " );
    }

    {
      String str(STR_input2);
      cs::replaceAll(str, "abc", "xyz");
      REQUIRE( str == "xyzxyz" );
    }
  }

  TEST_CASE("Split string at delimiter.", "[split]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *STR_split = "/abc//ABC/";

    {
      const StringList result = cs::split(String(STR_split), '/');

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.cbegin(), 0)->empty() );
      REQUIRE( *std::next(result.cbegin(), 1) == STR_abc );
      REQUIRE(  std::next(result.cbegin(), 2)->empty() );
      REQUIRE( *std::next(result.cbegin(), 3) == STR_ABC );
      REQUIRE(  std::next(result.cbegin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(String(STR_split), '/', true);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.cbegin(), 0) == STR_abc );
      REQUIRE( *std::next(result.cbegin(), 1) == STR_ABC );
    }

    const char *STR_split2 = "..abc....ABC..";

    {
      const StringList result = cs::split(String(STR_split2), "..");

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.cbegin(), 0)->empty() );
      REQUIRE( *std::next(result.cbegin(), 1) == STR_abc );
      REQUIRE(  std::next(result.cbegin(), 2)->empty() );
      REQUIRE( *std::next(result.cbegin(), 3) == STR_ABC );
      REQUIRE(  std::next(result.cbegin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(String(STR_split2), "..", true);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.cbegin(), 0) == STR_abc );
      REQUIRE( *std::next(result.cbegin(), 1) == STR_ABC );
    }
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
