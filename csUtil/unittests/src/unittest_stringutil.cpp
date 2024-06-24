#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch2/catch_test_macros.hpp>

#include <cs/Text/StringUtil.h>

namespace stringutil {

  using String     = std::string;
  using StringList = std::list<String>;

  TEST_CASE("Length of a string.", "[strlen]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_abcd    = "abcd";
    const char *PTR_null    = nullptr;
    const char *PTR_tainted = "abc\0e";

    REQUIRE( cs::strlen(&PTR_abcd[0], &PTR_abcd[4]) == 4 );
    REQUIRE( cs::strlen("abcd") == 4 );
    REQUIRE( cs::strlen("abcd", 4) == 4 );

    REQUIRE( cs::strlen(&PTR_tainted[0], &PTR_tainted[5]) == 3 );
    REQUIRE( cs::strlen(PTR_tainted) == 3 );
    REQUIRE( cs::strlen(PTR_tainted, 5) == 3 );

    REQUIRE( cs::strlen(PTR_null) == 0 );
    REQUIRE( cs::strlen("abc", 0) == 0 );
    REQUIRE( cs::strlen("abc", cs::MAX_SIZE_T) == 0 );
    REQUIRE( cs::strlen(PTR_null, 0) == 0 );
    REQUIRE( cs::strlen(PTR_null, cs::MAX_SIZE_T) == 0 );
    REQUIRE( cs::strlen("abc", PTR_null) == 0 );
    REQUIRE( cs::strlen(PTR_null, "abc") == 0 );
  }

  TEST_CASE("String contains pattern.", "[contains]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::contains("abcd", 'a') );
    REQUIRE( !cs::contains("abcd", 'A') );
    REQUIRE( !cs::contains(String(), 'a') );

    REQUIRE(  cs::contains("ABCD", "BC") );
    REQUIRE( !cs::contains("ABCD", "bc") );
    REQUIRE(  cs::contains("ABCD", "bc", true) );
    REQUIRE( !cs::contains(String(), "bc") );
    REQUIRE( !cs::contains("ABCD", String()) );

    REQUIRE(  cs::contains("Hello, World!", cs::lambda_is_space<char>()) );
    REQUIRE( !cs::contains("abcd", cs::lambda_is_space<char>()) );
    REQUIRE( !cs::contains(String(), cs::lambda_is_space<char>()) );
  }

  TEST_CASE("String ends with pattern.", "[endsWith]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::endsWith("abcd", "bcd") );
    REQUIRE( !cs::endsWith("abcd", "BCD") );
    REQUIRE(  cs::endsWith("abcd", "BCD", true) );

    REQUIRE(  cs::endsWith("ABCD", "BCD") );
    REQUIRE( !cs::endsWith("ABCD", "bcd") );
    REQUIRE(  cs::endsWith("ABCD", "bcd", true) );

    REQUIRE( !cs::endsWith("bcd", "abcd") );

    REQUIRE( !cs::endsWith(String(), "bcd") );
    REQUIRE( !cs::endsWith("abcd", String()) );
  }

  TEST_CASE("Two strings are equal.", "[equals]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::equals("abc", "abc") );
    REQUIRE( !cs::equals("abc", "ABC") );
    REQUIRE(  cs::equals("abc", "ABC", true) );
    REQUIRE( !cs::equals("abc", "abcd") );

    REQUIRE( !cs::equals(String(), "abc") );
    REQUIRE( !cs::equals("abc", String()) );
  }

  TEST_CASE("Classify string's contents.", "[classify]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::isHexString("0123456789abcdefABCDEF") );
    REQUIRE( !cs::isHexString("0123456789abcdefABCDEFx") );
    REQUIRE( !cs::isHexString(String()) );

    REQUIRE(  cs::isIdent("_") );
    REQUIRE( !cs::isIdent("0") );
    REQUIRE( !cs::isIdent(String()) );

    REQUIRE(  cs::isIdent("_azAZ09") );
    REQUIRE(  cs::isIdent("azAZ09_") );
    REQUIRE(  cs::isIdent("AZ09_az") );
    REQUIRE( !cs::isIdent("09_azAZ") );
    REQUIRE( !cs::isIdent("_azAZ09-") );
    REQUIRE( !cs::isIdent("_-azAZ09") );

    REQUIRE(  cs::isSpace(" \f\n\r\t\v") );
    REQUIRE( !cs::isSpace(" \f\n\r\t\v-") );
    REQUIRE( !cs::isSpace(String()) );
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

  TEST_CASE("Remove patterns in string.", "[removeAll]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const char *PTR_input1 = ".ab.cd.";
    const char *PTR_input2 = "abcabc";
    const char *PTR_input3 = " a b c ";

    {
      String str(PTR_input1);
      cs::removeAll(str, '.');
      REQUIRE( str == "abcd" );
    }

    {
      String str(PTR_input2);
      cs::removeAll(str, "abc");
      REQUIRE( str.size() == 0 );
    }

    {
      String str(PTR_input3);
      cs::removeAll(str, cs::lambda_is_space<char>());
      REQUIRE( str == "abc" );
    }
  }

  TEST_CASE("Remove trailing zeros.", "[removeTrailingZeros]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

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

    REQUIRE( cs::toSimplified("\fa\nb\rc\td\v") == "a b c d" );
    REQUIRE( cs::toSimplified("abcd") == "abcd" );
    REQUIRE( cs::toSimplified("\t\v\f\n\r ").empty() );
    REQUIRE( cs::toSimplified(String()).empty() );
  }

  TEST_CASE("Split string at delimiter.", "[split]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

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
      REQUIRE( *std::next(result.begin(), 1) == "abc" );
      REQUIRE(  std::next(result.begin(), 2)->empty() );
      REQUIRE( *std::next(result.begin(), 3) == "ABC" );
      REQUIRE(  std::next(result.begin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(PTR_split1, '/', cs::SplitFlag::SkipEmpty);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.begin(), 0) == "abc" );
      REQUIRE( *std::next(result.begin(), 1) == "ABC" );
    }

    const char *PTR_split2 = "..abc....ABC..";

    {
      const StringList result = cs::split(PTR_split2, "..");

      REQUIRE(  result.size() == 5 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == "abc" );
      REQUIRE(  std::next(result.begin(), 2)->empty() );
      REQUIRE( *std::next(result.begin(), 3) == "ABC" );
      REQUIRE(  std::next(result.begin(), 4)->empty() );
    }

    {
      const StringList result = cs::split(PTR_split2, "..", cs::SplitFlag::SkipEmpty);

      REQUIRE(  result.size() == 2 );
      REQUIRE( *std::next(result.begin(), 0) == "abc" );
      REQUIRE( *std::next(result.begin(), 1) == "ABC" );
    }

    const char *PTR_split3 = "\v: abc\t:\rABC\n:\f";

    {
      const StringList result = cs::split(PTR_split3, ':', cs::SplitFlag::Trim);

      REQUIRE(  result.size() == 4 );
      REQUIRE(  std::next(result.begin(), 0)->empty() );
      REQUIRE( *std::next(result.begin(), 1) == "abc" );
      REQUIRE( *std::next(result.begin(), 2) == "ABC" );
      REQUIRE(  std::next(result.begin(), 3)->empty() );
    }

    {
      const StringList result = cs::split(String(), ' ');

      REQUIRE( result.empty() );
    }

    {
      const StringList result = cs::split(String(), "..");

      REQUIRE( result.empty() );
    }

    {
      const StringList result = cs::split("abc", String());

      REQUIRE( result.empty() );
    }
  }

  TEST_CASE("String starts with pattern.", "[startsWith]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE(  cs::startsWith("abcd", "abc") );
    REQUIRE( !cs::startsWith("abcd", "ABC") );
    REQUIRE(  cs::startsWith("abcd", "ABC", true) );

    REQUIRE(  cs::startsWith("ABCD", "ABC") );
    REQUIRE( !cs::startsWith("ABCD", "abc") );
    REQUIRE(  cs::startsWith("ABCD", "abc", true) );

    REQUIRE( !cs::startsWith("abc", "abcd") );

    REQUIRE( !cs::startsWith(String(), "abc") );
    REQUIRE( !cs::startsWith("abcd", String()) );
  }

  TEST_CASE("Convert to lower case.", "[toLower]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::toLower("ABCD") == "abcd" );
    REQUIRE( cs::toLower(String()).empty() );
  }

  TEST_CASE("Convert to upper case.", "[toUpper]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::toUpper("abcd") == "ABCD" );
    REQUIRE( cs::toUpper(String()).empty() );
  }

  TEST_CASE("Trim string.", "[trim]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    REQUIRE( cs::toTrimmed("\nabc\r") == "abc" );
    REQUIRE( cs::toTrimmed("abc") == "abc" );
    REQUIRE( cs::toTrimmed("\t\v\f\n\r ").empty() );
    REQUIRE( cs::toTrimmed(String()).empty() );
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
