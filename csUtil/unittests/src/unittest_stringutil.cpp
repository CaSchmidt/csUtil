#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch2/catch_test_macros.hpp>

#include <cs/Text/StringUtil.h>

namespace stringutil {

  using String     = std::string;
  using StringList = std::list<String>;

  const char *PTR_abcd = "abcd";
  const char *PTR_abc  = "abc";
  const char *PTR_bcd  = "bcd";

  const char *PTR_ABCD = "ABCD";
  const char *PTR_ABC  = "ABC";
  const char *PTR_BCD  = "BCD";

  const char *PTR_tainted = "abc\0e";

  const char *PTR_null = nullptr;

  TEST_CASE("Length of a string.", "[strlen]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::strlen(&PTR_abcd[0], &PTR_abcd[4]) == 4 );
    REQUIRE( cs::strlen(PTR_abcd) == 4 );
    REQUIRE( cs::strlen(PTR_abcd, 4) == 4 );

    REQUIRE( cs::strlen(&PTR_tainted[0], &PTR_tainted[5]) == 3 );
    REQUIRE( cs::strlen(PTR_tainted) == 3 );
    REQUIRE( cs::strlen(PTR_tainted, 5) == 3 );

    REQUIRE( cs::strlen(PTR_null) == 0 );
    REQUIRE( cs::strlen(PTR_abc, 0) == 0 );
    REQUIRE( cs::strlen(PTR_abc, cs::MAX_SIZE_T) == 0 );
    REQUIRE( cs::strlen(PTR_null, 0) == 0 );
    REQUIRE( cs::strlen(PTR_null, cs::MAX_SIZE_T) == 0 );
    REQUIRE( cs::strlen(PTR_abc, PTR_null) == 0 );
    REQUIRE( cs::strlen(PTR_null, PTR_abc) == 0 );
  }

  TEST_CASE("String contains pattern.", "[contains]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::contains(PTR_abcd, 'a') );
    REQUIRE( !cs::contains(PTR_abcd, 'A') );
    REQUIRE(  cs::contains(PTR_ABCD, "BC") );
    REQUIRE( !cs::contains(PTR_ABCD, "bc") );
    REQUIRE(  cs::contains(PTR_ABCD, "bc", true) );
    REQUIRE(  cs::contains("Hello, World!", cs::lambda_is_space<char>()) );
    REQUIRE( !cs::contains(PTR_abcd, cs::lambda_is_space<char>()) );

    // StringUtil.h

    REQUIRE(  cs::contains(String(PTR_abcd), 'b') );
    REQUIRE( !cs::contains(String(PTR_abcd), 'B') );
    REQUIRE(  cs::contains(String(PTR_abcd), "bc") );
    REQUIRE( !cs::contains(String(PTR_abcd), "BC") );
    REQUIRE(  cs::contains(PTR_abcd, String("BC"), true) );
    REQUIRE(  cs::contains(String("Hello, World!"), cs::lambda_is_space<char>()) );
    REQUIRE( !cs::contains(String(PTR_abcd), cs::lambda_is_space<char>()) );
  }

  TEST_CASE("String ends with pattern.", "[endsWith]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::endsWith(PTR_abcd, PTR_bcd) );
    REQUIRE( !cs::endsWith(PTR_abcd, PTR_BCD) );
    REQUIRE(  cs::endsWith(PTR_abcd, PTR_BCD, true) );

    REQUIRE(  cs::endsWith(PTR_ABCD, PTR_BCD) );
    REQUIRE( !cs::endsWith(PTR_ABCD, PTR_bcd) );
    REQUIRE(  cs::endsWith(PTR_ABCD, PTR_bcd, true) );

    REQUIRE( !cs::endsWith(PTR_bcd, PTR_abcd) );

    // StringUtil.h

    REQUIRE(  cs::endsWith(String(PTR_abcd), PTR_bcd) );
    REQUIRE( !cs::endsWith(String(PTR_abcd), PTR_BCD) );
    REQUIRE(  cs::endsWith(String(PTR_abcd), PTR_BCD, true) );

    REQUIRE(  cs::endsWith(PTR_ABCD, String(PTR_BCD)) );
    REQUIRE( !cs::endsWith(String(PTR_ABCD), PTR_bcd) );
    REQUIRE(  cs::endsWith(String(PTR_ABCD), PTR_bcd, true) );

    REQUIRE( !cs::endsWith(String(PTR_bcd), PTR_abcd) );
  }

  TEST_CASE("Two strings are equal.", "[equals]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::equals(PTR_abc, PTR_abc) );
    REQUIRE( !cs::equals(PTR_abc, PTR_ABC) );
    REQUIRE(  cs::equals(PTR_abc, PTR_ABC, true) );
    REQUIRE( !cs::equals(PTR_abc, PTR_abcd) );

    // StringUtil.h

    REQUIRE(  cs::equals(String(PTR_abc), PTR_abc) );
    REQUIRE( !cs::equals(PTR_abc, String(PTR_ABC)) );
    REQUIRE(  cs::equals(String(PTR_abc), PTR_ABC, true) );
    REQUIRE( !cs::equals(String(PTR_abc), PTR_abcd) );
  }

  TEST_CASE("Classify string's contents.", "[classify]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::isHexString("0123456789abcdefABCDEF") );
    REQUIRE( !cs::isHexString("0123456789abcdefABCDEFx") );
    REQUIRE( !cs::isHexString("") );

    REQUIRE(  cs::isIdent("_") );
    REQUIRE( !cs::isIdent("0") );
    REQUIRE( !cs::isIdent("") );

    REQUIRE(  cs::isIdent("_azAZ09") );
    REQUIRE(  cs::isIdent("azAZ09_") );
    REQUIRE(  cs::isIdent("AZ09_az") );
    REQUIRE( !cs::isIdent("09_azAZ") );
    REQUIRE( !cs::isIdent("_azAZ09-") );
    REQUIRE( !cs::isIdent("_-azAZ09") );

    REQUIRE(  cs::isSpace(" \f\n\r\t\v") );
    REQUIRE( !cs::isSpace(" \f\n\r\t\v-") );
    REQUIRE( !cs::isSpace("") );

    // StringUtil.h

    REQUIRE(  cs::isHexString(String("0123456789abcdefABCDEF")) );
    REQUIRE( !cs::isHexString(String("0123456789abcdefABCDEFx")) );

    REQUIRE(  cs::isIdent(String("_")) );
    REQUIRE( !cs::isIdent(String("0")) );

    REQUIRE(  cs::isIdent(String("_azAZ09")) );
    REQUIRE(  cs::isIdent(String("azAZ09_")) );
    REQUIRE(  cs::isIdent(String("AZ09_az")) );
    REQUIRE( !cs::isIdent(String("09_azAZ")) );
    REQUIRE( !cs::isIdent(String("_azAZ09-")) );
    REQUIRE( !cs::isIdent(String("_-azAZ09")) );

    REQUIRE(  cs::isSpace(String(" \f\n\r\t\v")) );
    REQUIRE( !cs::isSpace(String(" \f\n\r\t\v-")) );
  }

  TEST_CASE("Detect digets in various radices.", "[isDigit]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr char succ9 = '9' + 1;

    REQUIRE(  cs::isDigit('0') );
    REQUIRE(  cs::isDigit('9') );
    REQUIRE( !cs::isDigit(succ9) );

    REQUIRE(  cs::isHexDigit('0') );
    REQUIRE(  cs::isHexDigit('9') );
    REQUIRE( !cs::isHexDigit(succ9) );

    REQUIRE(  cs::isHexDigit('a') );
    REQUIRE(  cs::isHexDigit('f') );
    REQUIRE( !cs::isHexDigit('g') );

    REQUIRE(  cs::isHexDigit('A') );
    REQUIRE(  cs::isHexDigit('F') );
    REQUIRE( !cs::isHexDigit('G') );

    REQUIRE(  cs::isOctDigit('0') );
    REQUIRE(  cs::isOctDigit('7') );
    REQUIRE( !cs::isOctDigit('8') );
  }

  TEST_CASE("Remove patterns string.", "[removeAll]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_input1 = ".ab.cd.";
    const char *PTR_input2 = "abcabc";
    const char *PTR_input3 = " a b c ";

    // StringUtil.h

    {
      String str(PTR_input1);
      cs::removeAll(str, '.');
      REQUIRE( str == PTR_abcd );
    }

    {
      String str(PTR_input2);
      cs::removeAll(str, PTR_abc);
      REQUIRE( str.size() == 0 );
    }

    {
      String str(PTR_input3);
      cs::removeAll(str, cs::lambda_is_space<char>());
      REQUIRE( str == PTR_abc );
    }
  }

  TEST_CASE("Remove trailing zeros.", "[removeTrailingZeros]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // StringUtil.h

    String s;

    s = "90";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90" );

    s = "90.0e0";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.0e0" );

    s = "90.0E0";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.0E0" );

    s = "90.999";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.999" );

    s = "90.";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90" );

    s = "90.0";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90" );

    s = "90.000";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90" );

    s = "90.009";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.009" );

    s = "90.090";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.09" );

    s = "90.90";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.9" );

    s = "90.900";
    cs::removeTrailingZeros(s);
    REQUIRE( s == "90.9" );

    s = "90.";
    cs::removeTrailingZeros(s, false);
    REQUIRE( s == "90." );

    s = "90.0";
    cs::removeTrailingZeros(s, false);
    REQUIRE( s == "90." );

    s = "90.000";
    cs::removeTrailingZeros(s, false);
    REQUIRE( s == "90." );
  }

  TEST_CASE("Replace part(s) of string.", "[replaceAll]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_input1 = " abc abc ";
    const char *PTR_input2 = "abcabc";

    // StringUtil.h

    {
      String str(PTR_input2);
      cs::replaceAll(str, 'c', 'x');
      REQUIRE( str == "abxabx" );
    }

    {
      String str(PTR_input1);
      cs::replaceAll(str, cs::lambda_is_space<char>(), '.');
      REQUIRE( str == ".abc.abc." );
    }

    {
      String str(PTR_input1);
      cs::replaceAll(str, ' ', ".");
      REQUIRE( str == ".abc.abc." );
    }

    {
      String str(PTR_input1);
      cs::replaceAll(str, "abc", "xyz");
      REQUIRE( str == " xyz xyz " );
    }

    {
      String str(PTR_input2);
      cs::replaceAll(str, "abc", "xyz");
      REQUIRE( str == "xyzxyz" );
    }
  }

  TEST_CASE("Simplification of a string.", "[simplify]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // StringUtil.h

    REQUIRE( cs::toSimplified(String("\fa\nb\rc\td\v")) == "a b c d" );
    REQUIRE( cs::toSimplified(String(PTR_abcd)) == PTR_abcd );
  }

  TEST_CASE("Split string at delimiter.", "[split]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // StringUtil.h

    const char *PTR_split1 = "/abc//ABC/";

    {
      const StringList result = cs::split(PTR_split1, 'x');

      REQUIRE(  result.size() == 1 );
      REQUIRE( *std::next(result.begin(), 0) == PTR_split1 );
    }

    {
      const StringList result = cs::split(PTR_split1, '/');

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == PTR_abc );
      REQUIRE(  std::next(result.begin(), 2)->empty() );
      REQUIRE( *std::next(result.begin(), 3) == PTR_ABC );
      REQUIRE(  std::next(result.begin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(PTR_split1, '/', cs::SplitFlag::SkipEmpty);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.begin(), 0) == PTR_abc );
      REQUIRE( *std::next(result.begin(), 1) == PTR_ABC );
    }

    const char *PTR_split2 = "..abc....ABC..";

    {
      const StringList result = cs::split(PTR_split2, "..");

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == PTR_abc );
      REQUIRE(  std::next(result.begin(), 2)->empty() );
      REQUIRE( *std::next(result.begin(), 3) == PTR_ABC );
      REQUIRE(  std::next(result.begin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(PTR_split2, "..", cs::SplitFlag::SkipEmpty);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.begin(), 0) == PTR_abc );
      REQUIRE( *std::next(result.begin(), 1) == PTR_ABC );
    }

    const char *PTR_split3 = "\v: abc\t:\rABC\n:\f";

    {
      const StringList result = cs::split(PTR_split3, ':', cs::SplitFlag::Trim);

      REQUIRE(  result.size() == 4 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == PTR_abc );
      REQUIRE( *std::next(result.begin(), 2) == PTR_ABC );
      REQUIRE(  std::next(result.begin(), 3)->empty() );
    }
  }

  TEST_CASE("String starts with pattern.", "[startsWith]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::startsWith(PTR_abcd, PTR_abc) );
    REQUIRE( !cs::startsWith(PTR_abcd, PTR_ABC) );
    REQUIRE(  cs::startsWith(PTR_abcd, PTR_ABC, true) );

    REQUIRE(  cs::startsWith(PTR_ABCD, PTR_ABC) );
    REQUIRE( !cs::startsWith(PTR_ABCD, PTR_abc) );
    REQUIRE(  cs::startsWith(PTR_ABCD, PTR_abc, true) );

    REQUIRE( !cs::startsWith(PTR_abc, PTR_abcd) );

    // StringUtil.h

    REQUIRE(  cs::startsWith(String(PTR_abcd), PTR_abc) );
    REQUIRE( !cs::startsWith(String(PTR_abcd), PTR_ABC) );
    REQUIRE(  cs::startsWith(String(PTR_abcd), PTR_ABC, true) );

    REQUIRE(  cs::startsWith(PTR_ABCD, String(PTR_ABC)) );
    REQUIRE( !cs::startsWith(String(PTR_ABCD), PTR_abc) );
    REQUIRE(  cs::startsWith(String(PTR_ABCD), PTR_abc, true) );

    REQUIRE( !cs::startsWith(String(PTR_abc), PTR_abcd) );
  }

  TEST_CASE("Convert to lower case.", "[toLower]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // StringUtil.h

    {
      String str(PTR_ABCD);
      cs::lower(str);
      REQUIRE( str == PTR_abcd );
    }
  }

  TEST_CASE("Convert to upper case.", "[toUpper]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // StringUtil.h

    {
      String str(PTR_abcd);
      cs::upper(str);
      REQUIRE( str == PTR_ABCD );
    }
  }

  TEST_CASE("Trim string.", "[trim]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // StringUtil.h

    REQUIRE( cs::toTrimmed(String("\nabc\r")) == PTR_abc );
    REQUIRE( cs::toTrimmed(String(PTR_abc)) == PTR_abc );
  }

  TEST_CASE("Narrowing/Widening of strings.", "[narrowwiden]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    { // narrow
      REQUIRE( cs::toString(u"Hello, World!") == "Hello, World!" );

      REQUIRE( cs::toString(L"Hello, World!") == "Hello, World!" );
    }

    { // widen
      REQUIRE( cs::toUtf16String("Hello, World!") == u"Hello, World!" );

      REQUIRE( cs::toWString("Hello, World!") == L"Hello, World!" );
    }
  } // TEST_CASE

} // namespace stringutil
