#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <iterator>

#include <catch.hpp>

#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringValue.h>

namespace stringvalue {

  TEST_CASE("String to integral value.", "[strtointegral]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using ValueT = int16_t;

    bool ok = false;

    { // empty string
      ok = false;
      REQUIRE( cs::toValue<ValueT>("", &ok) == ValueT{0} );
      REQUIRE( !ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("\t", &ok) == ValueT{0} );
      REQUIRE( !ok );
    }

    { // unsigned
      using ValueT = uint16_t;

      ok = false;
      REQUIRE( cs::toValue<ValueT>("\t\v\f\n\r 255  ", &ok) == ValueT{255} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("+255", &ok) == ValueT{255} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("-255", &ok) == ValueT{0} );
      REQUIRE( !ok );
    }

    { // signed decimal
      ok = false;
      REQUIRE( cs::toValue<ValueT>("+255", &ok) == ValueT{255} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("-255", &ok) == ValueT{-255} );
      REQUIRE( ok );
    }

    { // signed hexadecimal
      ok = false;
      REQUIRE( cs::toValue<ValueT>("fF", &ok, 16) == ValueT{255} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("+fF", &ok, 16) == ValueT{255} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("-fF", &ok, 16) == ValueT{-255} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("  +7fFF  ", &ok, 16) == ValueT{32767} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("  -8000  ", &ok, 16) == ValueT{-32768} );
      REQUIRE( ok );
    }
  } // TEST_CASE

  TEST_CASE("String to real value.", "[strtoreal]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using ValueT = double;

    bool ok = false;

    { // emtpy string
      ok = false;
      REQUIRE( cs::toValue<ValueT>("", &ok) == ValueT{0} );
      REQUIRE( !ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("\t", &ok) == ValueT{0} );
      REQUIRE( !ok );
    }

    { // decimal
      ok = false;
      REQUIRE( cs::toValue<ValueT>("\t\v\n\f\r +1.5", &ok) == ValueT{1.5} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("-0.25", &ok) == ValueT{-0.25} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("+1.5E2", &ok) == ValueT{150.0} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("-100e-2", &ok) == ValueT{-1.0} );
      REQUIRE( ok );
    }

    { // hexadecimal
      ok = false;
      REQUIRE( cs::toValue<ValueT>("1.8", &ok, 16) == ValueT{1.5} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("+1.8P+1", &ok, 16) == ValueT{3.0} );
      REQUIRE( ok );

      ok = false;
      REQUIRE( cs::toValue<ValueT>("-1.8p-1", &ok, 16) == ValueT{-0.75} );
      REQUIRE( ok );
    }
  } // TEST_CASE

} // namespace stringvalue
