#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <cs/Core/StringUtil.h>

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

  TEST_CASE("Beautification of a string.", "[beautification]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::trimmed(String("\nabc\r")) == STR_abc );
    REQUIRE( cs::trimmed(String(STR_abc)) == STR_abc );

    REQUIRE( cs::simplified(String("\fa\nb\rc\td\v")) == "a b c d" );
    REQUIRE( cs::simplified(String(STR_abcd)) == STR_abcd );
  }

  TEST_CASE("Case conversion.", "[case]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    {
      String str(STR_ABCD);
      cs::toLower(&str);
      REQUIRE( str == STR_abcd );
    }

    {
      String str(STR_abcd);
      cs::toUpper(&str);
      REQUIRE( str == STR_ABCD );
    }
  }

  TEST_CASE("Classify string's contents.", "[classify]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::isIdent("_") );
    REQUIRE( !cs::isIdent("0") );

    REQUIRE(  cs::isIdent("_azAZ09") );
    REQUIRE(  cs::isIdent("azAZ09_") );
    REQUIRE(  cs::isIdent("AZ09_az") );
    REQUIRE( !cs::isIdent("09_azAZ") );
    REQUIRE( !cs::isIdent("_azAZ09-") );
    REQUIRE( !cs::isIdent("_-azAZ09") );

    REQUIRE(  cs::isSpace(" \f\n\r\t\v") ); // C99
    REQUIRE( !cs::isSpace(" \f\n\r\t\v-") );
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

  TEST_CASE("Two strings are equal.", "[equals]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::equalsN(STR_abc, STR_abcd) );
    REQUIRE( !cs::equalsN(STR_abc, STR_ABCD) );
    REQUIRE(  cs::equalsN(STR_abc, STR_ABCD, true) );
    REQUIRE(  cs::equalsN(STR_ABC, STR_abcd, true) );

    REQUIRE(  cs::equals(STR_abc, STR_abc) );
    REQUIRE( !cs::equals(STR_abc, STR_ABC) );
    REQUIRE(  cs::equals(STR_abc, STR_ABC, true) );
    REQUIRE( !cs::equals(STR_abc, STR_abcd) );
  }

  TEST_CASE("Remove patterns string.", "[remove]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *STR_input1 = ".ab.cd.";
    const char *STR_input2 = "abcabc";
    const char *STR_input3 = " a b c ";

    {
      String str(STR_input1);
      cs::removeAll(&str, '.');
      REQUIRE( str == STR_abcd );
    }

    {
      String str(STR_input2);
      cs::removeAll(&str, STR_abc);
      REQUIRE( str.size() == 0 );
    }

    {
      String str(STR_input3);
      cs::removeAll(str.data(), str.data() + str.size(), cs::lambda_is_space<char>());
      cs::shrink(&str);
      REQUIRE( str == STR_abc );
    }

    {
      String str(STR_input3);
      cs::removeAll(&str, cs::lambda_is_space<char>());
      REQUIRE( str == STR_abc );
    }
  }

  TEST_CASE("Replace part(s) of string.", "[replace]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *STR_input1 = " abc abc ";
    const char *STR_input2 = "abcabc";

    {
      String str(STR_input1);
      cs::replaceAll(&str, ' ', ".");
      REQUIRE( str == ".abc.abc." );
    }

    {
      String str(STR_input1);
      cs::replaceAll(&str, "abc", "xyz");
      REQUIRE( str == " xyz xyz " );
    }

    {
      String str(STR_input2);
      cs::replaceAll(&str, "abc", "xyz");
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

  TEST_CASE("Remove trailing zeros.", "[remove]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    String s;

    s = "90";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90" );

    s = "90.0e0";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90.0e0" );

    s = "90.0E0";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90.0E0" );

    s = "90.999";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90.999" );

    s = "90.";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90" );

    s = "90.0";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90" );

    s = "90.000";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90" );

    s = "90.009";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90.009" );

    s = "90.90";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90.9" );

    s = "90.900";
    cs::removeTrailingZeros(&s);
    REQUIRE( s == "90.9" );

    s = "90.";
    cs::removeTrailingZeros(&s, false);
    REQUIRE( s == "90." );

    s = "90.0";
    cs::removeTrailingZeros(&s, false);
    REQUIRE( s == "90." );

    s = "90.000";
    cs::removeTrailingZeros(&s, false);
    REQUIRE( s == "90." );
  }

} // namespace stringutil
