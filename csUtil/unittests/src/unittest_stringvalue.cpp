#include <cstdio>
#include <cstdlib>

#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <cs/Text/StringValue.h>

namespace util {

  template<typename T>
  inline std::string to_hexstr(const T value)
  {
#if 0
    std::ostringstream stream;
    stream << std::hex << value;
    return stream.str();
#else
    std::string str(128, '\0');
    snprintf(str.data(), str.size(), "%x", value);
    cs::impl_strval::shrink(str, true);
    while( str.size() > sizeof(T)*2 ) {
      str.erase(0, 1);
    }
    return str;
#endif
  }

  template<typename T>
  inline std::string to_octstr(const T value)
  {
#if 0
    std::ostringstream stream;
    stream << std::oct << value;
    return stream.str();
#else
    std::string str(128, '\0');
    snprintf(str.data(), str.size(), "%o", value);
    cs::impl_strval::shrink(str, true);
    return str;
#endif
  }

  template<typename T>
  inline cs::if_real_t<T,void> print(const T value,
                                     const char fmt,
                                     const int prec)
  {
    std::cout << "value = ";
    std::cout << value;
    std::cout << " (std) vs. ";
    std::cout << cs::toString(value, fmt, prec);
    std::cout << " (";
    std::cout << "format = " << fmt;
    std::cout << ", ";
    std::cout << "precision = " << prec;
    std::cout << ")";
    std::cout << std::endl;
  }

  template<typename T>
  inline cs::if_real_t<T,void> print3(const T value,
                                      const char fmt)
  {
    print(value, fmt, 0);
    print(value, fmt, 3);
    print(value, fmt, 6);
    std::cout << std::endl;
  }

} // namespace util

namespace stringvalue {

  TEST_CASE("Integral value to string.", "[integraltostr]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using ValueT = int16_t;

    constexpr auto MAX = std::numeric_limits<ValueT>::max();
    constexpr auto MIN = std::numeric_limits<ValueT>::lowest();

    { // decimal
      REQUIRE( cs::toString<ValueT>(0) == "0" );

      REQUIRE( cs::toString<ValueT>(1) == "1" );

      REQUIRE( cs::toString<ValueT>(-1) == "-1" );

      REQUIRE( cs::toString<ValueT>(MAX) == "32767" );

      REQUIRE( cs::toString<ValueT>(MIN) == "-32768" );
    }

    { // hexadecimal
      REQUIRE( cs::toString<ValueT>(0, 16) == "0" );

      REQUIRE( cs::toString<ValueT>(1, 16) == "1" );

      REQUIRE( cs::toString<ValueT>(-1, 16) == "ffff" );

      REQUIRE( cs::toString<ValueT>(MAX, 16) == "7fff" );

      REQUIRE( cs::toString<ValueT>(MIN, 16) == "8000" );
    }

    { // octal
      REQUIRE( cs::toString<ValueT>(0, 8) == "0" );

      REQUIRE( cs::toString<ValueT>(1, 8) == "1" );

      REQUIRE( cs::toString<ValueT>(-1, 8) == "177777" );

      REQUIRE( cs::toString<ValueT>(MAX, 8) == "77777" );

      REQUIRE( cs::toString<ValueT>(MIN, 8) == "100000" );
    }
  } // TEST_CASE

  TEST_CASE("Real value to string.", "[realtostr]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using ValueT = double;

    util::print3(ValueT{1250}, 'e');
    util::print3(ValueT{1500}, 'e');

    util::print3(1.25, 'f');
    util::print3(1.5, 'f');

    { // fixed
      REQUIRE( cs::toString(1.25, 'f', 0) == "1" );
      REQUIRE( cs::toString(1.25, 'f', 3) == "1.250" );
      REQUIRE( cs::toString(1.25, 'f', 6) == "1.250000" );

      REQUIRE( cs::toString(1.5, 'f', 0) == "2" );
      REQUIRE( cs::toString(1.5, 'f', 3) == "1.500" );
      REQUIRE( cs::toString(1.5, 'f', 6) == "1.500000" );
    }
  }

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
