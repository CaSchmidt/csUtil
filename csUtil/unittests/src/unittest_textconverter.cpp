#include <cstdio>
#include <cstdlib>

#include <csUtil/csTextConverter.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#define TEST_UTF8_STRING  "für€²³°"

namespace test_unicode {

  TEST_CASE("Convert UTF-8 string to Unicode.", "[unicode]") {
    const csTextConverter conv = csTextConverter::createUtf8();
    REQUIRE( !conv.isNull() );

    const std::u16string unicode = conv.toUnicode(TEST_UTF8_STRING);
    REQUIRE( unicode.size() == 7 );

    REQUIRE( unicode[0] == u'f' );
    REQUIRE( unicode[1] == u'\xFC' );
    REQUIRE( unicode[2] == u'r' );
    REQUIRE( unicode[3] == u'\x20AC' );
    REQUIRE( unicode[4] == u'\xB2' );
    REQUIRE( unicode[5] == u'\xB3' );
    REQUIRE( unicode[6] == u'\xB0' );
  }

} // test_unicode

namespace test_ascii {

  TEST_CASE("Convert Unicode string to ASCII.", "[ascii]") {
    const std::u16string unicode = csUtf8ToUnicode(TEST_UTF8_STRING);

    const csTextConverter conv = csTextConverter::createAscii();
    REQUIRE( !conv.isNull() );

    const std::string ascii = conv.fromUnicode(unicode);
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
    const std::u16string unicode = csUtf8ToUnicode(TEST_UTF8_STRING);

    const csTextConverter conv = csTextConverter::createLatin1();
    REQUIRE( !conv.isNull() );

    const std::string latin1 = conv.fromUnicode(unicode);
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
    const std::u16string unicode = csUtf8ToUnicode(TEST_UTF8_STRING);

    const csTextConverter conv = csTextConverter::createLatin9();
    REQUIRE( !conv.isNull() );

    const std::string latin9 = conv.fromUnicode(unicode);
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

namespace test_windows1252 {

  TEST_CASE("Convert Unicode string to Windows-1252.", "[windows1252]") {
    const std::u16string unicode = csUtf8ToUnicode(TEST_UTF8_STRING);

    const csTextConverter conv = csTextConverter::createWindows1252();
    REQUIRE( !conv.isNull() );

    const std::string windows1252 = conv.fromUnicode(unicode);
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
