#include <iterator>
#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/IO/File.h>
#include <cs/System/PathFormatter.h>
#include <cs/Text/StringUtil.h>
#include <cs/Text/TextIO.h>

namespace test_fileio {

  using String     = std::string;
  using StringList = std::list<String>;

  TEST_CASE("Read lines from text file.", "[lines]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

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
      REQUIRE(  std::next(lines.begin(), 0)->empty() );
      REQUIRE( *std::next(lines.begin(), 1) == " abc " );
      REQUIRE(  std::next(lines.begin(), 2)->empty() );
      REQUIRE( *std::next(lines.begin(), 3) == " xyz " );
      REQUIRE( *std::next(lines.begin(), 4) == "   " );
      REQUIRE(  std::next(lines.begin(), 5)->empty() );
    }

    // (3) Read & trim lines /////////////////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename, cs::LineFlag::Trim);
      REQUIRE(  lines.size() == 6 );
      REQUIRE(  std::next(lines.begin(), 0)->empty() );
      REQUIRE( *std::next(lines.begin(), 1) == "abc" );
      REQUIRE(  std::next(lines.begin(), 2)->empty() );
      REQUIRE( *std::next(lines.begin(), 3) == "xyz" );
      REQUIRE(  std::next(lines.begin(), 4)->empty() );
      REQUIRE(  std::next(lines.begin(), 5)->empty() );
    }

    // (4) Read & skip blank/empty lines /////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename, cs::LineFlag::SkipBlank);
      REQUIRE(  lines.size() == 2 );
      REQUIRE( *std::next(lines.begin(), 0) == " abc " );
      REQUIRE( *std::next(lines.begin(), 1) == " xyz " );
    }

    // (5) Read, trim & skip blank lines /////////////////////////////////////

    {
      const StringList lines = cs::readLines(filename, cs::LineFlag::All);
      REQUIRE(  lines.size() == 2 );
      REQUIRE( *std::next(lines.begin(), 0) == "abc" );
      REQUIRE( *std::next(lines.begin(), 1) == "xyz" );
    }
  }

} // namespace test_fileio

namespace test_path {

  TEST_CASE("Formatting of std::filesystem::path.", "[format]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const std::filesystem::path p("/path/to/file/filename.txt");

    REQUIRE( std::format("{}", p.filename()) == "filename.txt" );

    REQUIRE( std::format(L"{}", p.filename()) == L"filename.txt" );
  }

} // namespace test_path
