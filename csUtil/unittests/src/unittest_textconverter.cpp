#include <print>

#include <cs/Unicode/TextConverter.h>

#include <catch2/catch_test_macros.hpp>

#define TEST_UTF8_STRING  "für€²³°"
#define TEST_UTF16_STRING  u"für€²³°"

namespace test_unicode {

  TEST_CASE("Convert UTF-8 string to Unicode.", "[unicode]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::TextConverter conv = cs::TextConverter::createUtf8();
    REQUIRE( !conv.isNull() );

    const std::u16string unicode = conv.toUnicode(TEST_UTF8_STRING);
    REQUIRE( unicode.size() == 7 );

    REQUIRE( unicode[0] == u'f' );
    REQUIRE( unicode[1] == u'\u00FC' );
    REQUIRE( unicode[2] == u'r' );
    REQUIRE( unicode[3] == u'\u20AC' );
    REQUIRE( unicode[4] == u'\u00B2' );
    REQUIRE( unicode[5] == u'\u00B3' );
    REQUIRE( unicode[6] == u'\u00B0' );
  }

} // test_unicode

namespace test_ascii {

  TEST_CASE("Convert Unicode string to ASCII.", "[ascii]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::TextConverter conv = cs::TextConverter::createAscii();
    REQUIRE( !conv.isNull() );

    const std::string ascii = conv.fromUnicode(TEST_UTF16_STRING);
    REQUIRE( ascii.size() == 7 );

    REQUIRE( ascii[0] == 'f' );
    REQUIRE( ascii[1] == '\x1A' );
    REQUIRE( ascii[2] == 'r' );
    REQUIRE( ascii[3] == '\x1A' );
    REQUIRE( ascii[4] == '\x1A' );
    REQUIRE( ascii[5] == '\x1A' );
    REQUIRE( ascii[6] == '\x1A' );
  }

} // test_ascii

namespace test_latin1 {

  TEST_CASE("Convert Unicode string to ISO 8859-1 / Latin-1.", "[latin1]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::TextConverter conv = cs::TextConverter::createLatin1();
    REQUIRE( !conv.isNull() );

    const std::string latin1 = conv.fromUnicode(TEST_UTF16_STRING);
    REQUIRE( latin1.size() == 7 );

    REQUIRE( latin1[0] == 'f' );
    REQUIRE( latin1[1] == '\xFC' );
    REQUIRE( latin1[2] == 'r' );
    REQUIRE( latin1[3] == '\x1A' );
    REQUIRE( latin1[4] == '\xB2' );
    REQUIRE( latin1[5] == '\xB3' );
    REQUIRE( latin1[6] == '\xB0' );
  }

} // test_latin1

namespace test_latin9 {

  TEST_CASE("Convert Unicode string to ISO 8859-15 / Latin-9.", "[latin9]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::TextConverter conv = cs::TextConverter::createLatin9();
    REQUIRE( !conv.isNull() );

    const std::string latin9 = conv.fromUnicode(TEST_UTF16_STRING);
    REQUIRE( latin9.size() == 7 );

    REQUIRE( latin9[0] == 'f' );
    REQUIRE( latin9[1] == '\xFC' );
    REQUIRE( latin9[2] == 'r' );
    REQUIRE( latin9[3] == '\xA4' );
    REQUIRE( latin9[4] == '\xB2' );
    REQUIRE( latin9[5] == '\xB3' );
    REQUIRE( latin9[6] == '\xB0' );
  }

} // test_latin9

// cf. https://www.utf8-zeichentabelle.de/

namespace test_utf8 {

  TEST_CASE("Convert Unicode string to UTF-8.", "[utf8]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::TextConverter conv = cs::TextConverter::createUtf8();
    REQUIRE( !conv.isNull() );

    const std::string utf8 = conv.fromUnicode(TEST_UTF16_STRING);
    REQUIRE( utf8.size() == 13 );

    REQUIRE( utf8[ 0] == 'f' );
    REQUIRE( utf8[ 1] == '\xC3' ); // ü
    REQUIRE( utf8[ 2] == '\xBC' );
    REQUIRE( utf8[ 3] == 'r' );
    REQUIRE( utf8[ 4] == '\xE2' ); // €
    REQUIRE( utf8[ 5] == '\x82' );
    REQUIRE( utf8[ 6] == '\xAC' );
    REQUIRE( utf8[ 7] == '\xC2' ); // ²
    REQUIRE( utf8[ 8] == '\xB2' );
    REQUIRE( utf8[ 9] == '\xC2' ); // ³
    REQUIRE( utf8[10] == '\xB3' );
    REQUIRE( utf8[11] == '\xC2' ); // °
    REQUIRE( utf8[12] == '\xB0' );
  }

} // test_utf8

namespace test_windows1252 {

  TEST_CASE("Convert Unicode string to Windows-1252.", "[windows1252]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::TextConverter conv = cs::TextConverter::createWindows1252();
    REQUIRE( !conv.isNull() );

    const std::string windows1252 = conv.fromUnicode(TEST_UTF16_STRING);
    REQUIRE( windows1252.size() == 7 );

    REQUIRE( windows1252[0] == 'f' );
    REQUIRE( windows1252[1] == '\xFC' );
    REQUIRE( windows1252[2] == 'r' );
    REQUIRE( windows1252[3] == '\x80' );
    REQUIRE( windows1252[4] == '\xB2' );
    REQUIRE( windows1252[5] == '\xB3' );
    REQUIRE( windows1252[6] == '\xB0' );
  }

} // test_windows1252
