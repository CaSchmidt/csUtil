#include <algorithm>
#include <format>
#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Convert/BufferUtil.h>

namespace test_bytearray {

  TEST_CASE("Conversion from/to hexadecimal string.", "[hexstring]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    { // Case 1 - Even Length
      const std::string  s1 = std::format("{:X}", uint16_t{0xabcd});
      const cs::Buffer    a = cs::fromHexString(s1).value();
      const std::string  s2 = cs::toHexString(a, '\0', true).value();

      REQUIRE( s1 == s2 );
    }

    { // Case 2 - Odd Length
      const std::string  s1 = std::format("{:X}", uint32_t{0xabcde});
      const auto          a = cs::fromHexString(s1);

      REQUIRE( !a.has_value() );
    }

    { // Case 3 - Even Length with Fill
      const std::string  s1 = std::format("{:x}", uint16_t{0xabcd});
      const cs::Buffer    a = cs::fromHexString(s1).value();
      const std::string  s2 = cs::toHexString(a, '-').value();

      REQUIRE( "ab-cd" == s2 );
    }
  } // TEST_CASE

} // namespace test_bytearray

namespace test_bufferview {

  TEST_CASE("Operations on BufferView.", "[bufferview]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::BufferView view("Hello", 5);

    {
      REQUIRE( view.front() == 'H' );
      REQUIRE( view.back() == 'o' );

      REQUIRE( view[2] == view[3] );
    }

    {
      std::string s;
      std::copy(view.begin(), view.end(), std::back_inserter(s));
      std::println("{}", s);

      REQUIRE( s == "Hello" );
    }

    {
      std::string s;
      std::copy(view.rbegin(), view.rend(), std::back_inserter(s));
      std::println("{}", s);

      REQUIRE( s == "olleH" );
    }

    {
      REQUIRE( cs::toStringView(view.subview(5)).empty() );

      REQUIRE( cs::toStringView(view.subview(2, 2)) == "ll" );

      REQUIRE( cs::toStringView(view.subview(4, 5)) == "o" );

      REQUIRE( cs::toStringView(view.subview(4)) == "o" );
    }

    {
      const cs::BufferView subview(std::next(view.begin(), 2),
                                   std::next(view.begin(), 4));

      REQUIRE( cs::toStringView(subview) == "ll" );
    }
  } // TEST_CASE

} // namespace test_bufferview
