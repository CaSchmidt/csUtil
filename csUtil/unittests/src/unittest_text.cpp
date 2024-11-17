#include <iterator>
#include <print>

#include <catch2/catch_test_macros.hpp>

#include <cs/Text/KeyValue.h>
#include <cs/Text/MixIn.h>

namespace test_keyvalue {

  TEST_CASE("Make key-value pairs.", "[keyvalue]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const cs::KeyValuePairs<char> pairs
        = cs::makeKeyValuePairs(",,key1:value1,,:value2,key3:,,key4:value4,key5,,");

    REQUIRE( pairs.size() == 4 );

    const auto p1 = std::next(pairs.begin(), 0);
    REQUIRE( p1->first  == "key1" );
    REQUIRE( p1->second == "value1" );

    const auto p3 = std::next(pairs.begin(), 1);
    REQUIRE( p3->first == "key3" );
    REQUIRE( p3->second.empty() );

    const auto p4 = std::next(pairs.begin(), 2);
    REQUIRE( p4->first  == "key4" );
    REQUIRE( p4->second == "value4" );

    const auto p5 = std::next(pairs.begin(), 3);
    REQUIRE( p5->first == "key5" );
    REQUIRE( p5->second.empty() );
  } // TEST_CASE

} // namespace test_keyvalue

struct Object : public cs::ToStringMixIn<Object> {
  Object(const unsigned int a, const int b) noexcept
    : a{a}
    , b{b}
  {
  }

  auto elements() const
  {
    return std::forward_as_tuple(a, b);
  }

  unsigned int a{0};
  int b{0};
};

namespace std {

  template<typename CharT>
  struct formatter<Object,CharT> : public cs::ToString_formatter<Object,CharT> {
  };

} // namespace std

namespace test_mixin {

  TEST_CASE("Render object to string.", "[mixin]") {
    std::println("*** {}", Catch::getResultCapture().getCurrentTestName());

    const Object obj(7, -1);

    REQUIRE( obj.toString()  ==  "7, -1" );
    REQUIRE( obj.toWString() == L"7, -1" );

    REQUIRE( std::format( "{}", obj) ==  "7, -1" );
    REQUIRE( std::format(L"{}", obj) == L"7, -1" );
  } // TEST_CASE

} // namespace test_mixin
