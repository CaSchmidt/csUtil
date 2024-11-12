#include <cstdio>
#include <cstdlib>

#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Core/Range.h>
#include <cs/IO/BufferedReader.h>
#include <cs/IO/File.h>

namespace bufferedreader {

  TEST_CASE("Read buffered file line-by-line.", "[stringreader]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

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
      output.write(test_data, cs::strlen(test_data));
    }

    // (2) Bufferd Input /////////////////////////////////////////////////////

    cs::File input;
    REQUIRE( input.open(filename) );

    cs::BufferedReader reader{SIZE_BUFFER};
    reader.reset();
    REQUIRE( !reader.isEmpty() );

    // Line 1
    const std::string line1 = reader.getLine(input);
    REQUIRE( line1.empty() );
    // Line 2
    const std::string line2 = reader.getLine(input);
    REQUIRE( line2 == "01234567" );
    // Line 3.1 (NOTE: Line DOES NOT fit into buffer!)
    const std::string line31 = reader.getLine(input);
    REQUIRE( line31 == "0123456789ABCDEF" );
    // Line 3.2
    const std::string line32 = reader.getLine(input);
    REQUIRE( line32.empty() );
    // Line 4
    const std::string line4 = reader.getLine(input);
    REQUIRE( line4.empty() );
    // Line 5
    const std::string line5 = reader.getLine(input);
    REQUIRE( line5 == "01234567" );
    // Line 6
    const std::string line6 = reader.getLine(input);
    REQUIRE( line6.empty() );
    REQUIRE( input.atEnd() );
  }

} // namespace bufferedreader
