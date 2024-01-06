#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <cs/Text/StringUtil.h>

namespace stringutil {

  using String     = cs::String<char>;
  using StringList = cs::StringList<char>;

  const char *PTR_abcd = "abcd";
  const char *PTR_abc  = "abc";
  const char *PTR_bcd  = "bcd";

  const char *PTR_ABCD = "ABCD";
  const char *PTR_ABC  = "ABC";
  const char *PTR_BCD  = "BCD";

  const char *PTR_tainted = "abc\0e";

  TEST_CASE("Length of a string.", "[length]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::strlen(&PTR_abcd[0], &PTR_abcd[4]) == 4 );
    REQUIRE( cs::strlen(PTR_abcd) == 4 );
    REQUIRE( cs::strlen(PTR_abcd, 4) == 4 );

    REQUIRE( cs::strlen(&PTR_tainted[0], &PTR_tainted[5]) == 3 );
    REQUIRE( cs::strlen(PTR_tainted) == 3 );
    REQUIRE( cs::strlen(PTR_tainted, 5) == 3 );
  }

  TEST_CASE("Beautification of a string.", "[beautification]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::trimmed(String("\nabc\r")) == PTR_abc );
    REQUIRE( cs::trimmed(String(PTR_abc)) == PTR_abc );

    REQUIRE( cs::simplified(String("\fa\nb\rc\td\v")) == "a b c d" );
    REQUIRE( cs::simplified(String(PTR_abcd)) == PTR_abcd );
  }

  TEST_CASE("Case conversion.", "[case]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    {
      String str(PTR_ABCD);
      cs::toLower(&str);
      REQUIRE( str == PTR_abcd );
    }

    {
      String str(PTR_abcd);
      cs::toUpper(&str);
      REQUIRE( str == PTR_ABCD );
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

  TEST_CASE("String contains pattern.", "[contains]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::contains(PTR_abcd, 'a') );
    REQUIRE( !cs::contains(PTR_abcd, 'A') );
    REQUIRE(  cs::contains("Hello, World!", cs::lambda_is_space<char>()) );
    REQUIRE( !cs::contains(PTR_abcd, cs::lambda_is_space<char>()) );
    REQUIRE(  cs::contains(PTR_ABCD, "BC") );
    REQUIRE( !cs::contains(PTR_ABCD, "bc") );
    REQUIRE(  cs::contains(PTR_ABCD, "bc", true) );
  }

  TEST_CASE("String ends with pattern.", "[ends]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::endsWith(PTR_abcd, PTR_bcd) );
    REQUIRE( !cs::endsWith(PTR_abcd, PTR_BCD) );
    REQUIRE(  cs::endsWith(PTR_abcd, PTR_BCD, true) );

    REQUIRE(  cs::endsWith(PTR_ABCD, PTR_BCD) );
    REQUIRE( !cs::endsWith(PTR_ABCD, PTR_bcd) );
    REQUIRE(  cs::endsWith(PTR_ABCD, PTR_bcd, true) );

    REQUIRE( !cs::endsWith(PTR_bcd, PTR_abcd) );
  }

  TEST_CASE("Two strings are equal.", "[equals]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::equals(PTR_abc, PTR_abc) );
    REQUIRE( !cs::equals(PTR_abc, PTR_ABC) );
    REQUIRE(  cs::equals(PTR_abc, PTR_ABC, true) );
    REQUIRE( !cs::equals(PTR_abc, PTR_abcd) );
  }

  TEST_CASE("Remove patterns string.", "[remove]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_input1 = ".ab.cd.";
    const char *PTR_input2 = "abcabc";
    const char *PTR_input3 = " a b c ";

    {
      String str(PTR_input1);
      cs::removeAll(&str, '.');
      REQUIRE( str == PTR_abcd );
    }

    {
      String str(PTR_input2);
      cs::removeAll(&str, PTR_abc);
      REQUIRE( str.size() == 0 );
    }

    {
      String str(PTR_input3);
      cs::removeAll(str.data(), str.size(), cs::lambda_is_space<char>());
      cs::shrink(&str);
      REQUIRE( str == PTR_abc );
    }

    {
      String str(PTR_input3);
      cs::removeAll(&str, cs::lambda_is_space<char>());
      REQUIRE( str == PTR_abc );
    }
  }

  TEST_CASE("Replace part(s) of string.", "[replace]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_input1 = " abc abc ";
    const char *PTR_input2 = "abcabc";

    {
      String str(PTR_input1);
      cs::replaceAll(&str, ' ', ".");
      REQUIRE( str == ".abc.abc." );
    }

    {
      String str(PTR_input1);
      cs::replaceAll(&str, "abc", "xyz");
      REQUIRE( str == " xyz xyz " );
    }

    {
      String str(PTR_input2);
      cs::replaceAll(&str, "abc", "xyz");
      REQUIRE( str == "xyzxyz" );
    }

    {
      String str{PTR_input2};
      cs::replaceAll(&str, 'c', 'x');
      REQUIRE( str == "abxabx" );
    }

    {
      String str{PTR_input1};
      cs::replaceAll(&str, cs::lambda_is_space<char>(), '.');
      REQUIRE( str == ".abc.abc." );
    }
  }

  TEST_CASE("Split string at delimiter.", "[split]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_split = "/abc//ABC/";

    {
      const StringList result = cs::split(PTR_split, 'x');

      REQUIRE(  result.size() == 1 );
      REQUIRE( *std::next(result.begin(), 0) == PTR_split );
    }

    {
      const StringList result = cs::split(String(PTR_split), '/');

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == PTR_abc );
      REQUIRE(  std::next(result.begin(), 2)->empty() );
      REQUIRE( *std::next(result.begin(), 3) == PTR_ABC );
      REQUIRE(  std::next(result.begin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(String(PTR_split), '/', true);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.begin(), 0) == PTR_abc );
      REQUIRE( *std::next(result.begin(), 1) == PTR_ABC );
    }

    const char *PTR_split2 = "..abc....ABC..";

    {
      const StringList result = cs::split(String(PTR_split2), "..");

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == PTR_abc );
      REQUIRE(  std::next(result.begin(), 2)->empty() );
      REQUIRE( *std::next(result.begin(), 3) == PTR_ABC );
      REQUIRE(  std::next(result.begin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(String(PTR_split2), "..", true);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.begin(), 0) == PTR_abc );
      REQUIRE( *std::next(result.begin(), 1) == PTR_ABC );
    }
  }

  TEST_CASE("String starts with pattern.", "[starts]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::startsWith(PTR_abcd, PTR_abc) );
    REQUIRE( !cs::startsWith(PTR_abcd, PTR_ABC) );
    REQUIRE(  cs::startsWith(PTR_abcd, PTR_ABC, true) );

    REQUIRE(  cs::startsWith(PTR_ABCD, PTR_ABC) );
    REQUIRE( !cs::startsWith(PTR_ABCD, PTR_abc) );
    REQUIRE(  cs::startsWith(PTR_ABCD, PTR_abc, true) );

    REQUIRE( !cs::startsWith(PTR_abc, PTR_abcd) );
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
