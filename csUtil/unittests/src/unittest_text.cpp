#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <cs/Text/KeyValue.h>

namespace text {

  TEST_CASE("Make key-value pairs.", "[keyvalue]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    const cs::KeyValuePairs<char> pairs
        = cs::makeKVPairs(",,key1:value1,,:value2,key3:,,key4:value4,key5,,");

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
  }

} // namespace text
