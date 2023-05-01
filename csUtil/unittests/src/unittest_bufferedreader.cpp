#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <cs/IO/BufferedReader.h>
#include <cs/IO/File.h>
#include <cs/Text/StringRange.h>

namespace bufferedreader {

  TEST_CASE("Read buffered file line-by-line.", "[stringreader]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr std::size_t SIZE_BUFFER = 16;

    const char8_t *filename = u8"./BufferedReader.txt";

    const char *test_data =
        "\n"
        "01234567\n"
        "0123456789ABCDEF\n"
        "\n"
        "01234567\n";

    // (1) Setup /////////////////////////////////////////////////////////////

    {
      cs::File output;
      REQUIRE( output.open(filename, cs::FileOpenFlag::Write) );
      output.write(test_data, cs::length(test_data));
    }

    // (2) Bufferd Input /////////////////////////////////////////////////////

    cs::File input;
    REQUIRE( input.open(filename) );

    cs::BufferedReader reader{SIZE_BUFFER};
    reader.reset();
    REQUIRE( !reader.isEmpty() );

    std::string_view view;

    // Line 1
    REQUIRE( reader.getLine(&view, &input) );
    REQUIRE( view.empty() );
    // Line 2
    REQUIRE( reader.getLine(&view, &input) );
    REQUIRE( view == "01234567" );
    // Line 3.1 (NOTE: Line DOES NOT fit into buffer!)
    REQUIRE( reader.getLine(&view, &input) );
    REQUIRE( view == "0123456789ABCDEF" );
    // Line 3.2
    REQUIRE( reader.getLine(&view, &input) );
    REQUIRE( view.empty() );
    // Line 4
    REQUIRE( reader.getLine(&view, &input) );
    REQUIRE( view.empty() );
    // Line 5
    REQUIRE( reader.getLine(&view, &input) );
    REQUIRE( view == "01234567" );
    // Line 6 (NOTE: NOT returned!)
    REQUIRE( !reader.getLine(&view, &input) );
    REQUIRE( view.empty() );
  }

} // namespace bufferedreader
