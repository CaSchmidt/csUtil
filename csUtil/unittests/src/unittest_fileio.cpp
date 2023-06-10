#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <cs/IO/File.h>
#include <cs/Text/StringUtil.h>
#include <cs/Text/TextIO.h>

namespace fileio {

  using String     = std::string;
  using StringList = std::list<String>;

  TEST_CASE("Read lines from text file.", "[lines]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    // (1) Setup /////////////////////////////////////////////////////////////

    const char8_t *filename = u8"./csReadLines.txt";

    cs::File file;
    REQUIRE( file.open(filename, cs::FileOpenFlag::Write) );

    const char *STR_lines = "\n abc \n\n xyz \n   \n";
    file.write(STR_lines, cs::strlen(STR_lines));

    file.close();

    // (2) Read lines as-is //////////////////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename);
      REQUIRE(  lines.size() == 6 );
      REQUIRE(  std::next(lines.cbegin(), 0)->empty() );
      REQUIRE( *std::next(lines.cbegin(), 1) == " abc " );
      REQUIRE(  std::next(lines.cbegin(), 2)->empty() );
      REQUIRE( *std::next(lines.cbegin(), 3) == " xyz " );
      REQUIRE( *std::next(lines.cbegin(), 4) == "   " );
      REQUIRE(  std::next(lines.cbegin(), 5)->empty() );
    }

    // (3) Read & trim lines /////////////////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename, false, true);
      REQUIRE(  lines.size() == 6 );
      REQUIRE(  std::next(lines.cbegin(), 0)->empty() );
      REQUIRE( *std::next(lines.cbegin(), 1) == "abc" );
      REQUIRE(  std::next(lines.cbegin(), 2)->empty() );
      REQUIRE( *std::next(lines.cbegin(), 3) == "xyz" );
      REQUIRE(  std::next(lines.cbegin(), 4)->empty() );
      REQUIRE(  std::next(lines.cbegin(), 5)->empty() );
    }

    // (4) Read & skip blank/empty lines /////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename, true, false);
      REQUIRE(  lines.size() == 2 );
      REQUIRE( *std::next(lines.cbegin(), 0) == " abc " );
      REQUIRE( *std::next(lines.cbegin(), 1) == " xyz " );
    }

    // (5) Read, trim & skip blank lines /////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename, true, true);
      REQUIRE(  lines.size() == 2 );
      REQUIRE( *std::next(lines.cbegin(), 0) == "abc" );
      REQUIRE( *std::next(lines.cbegin(), 1) == "xyz" );
    }
  }

} // namespace fileio
