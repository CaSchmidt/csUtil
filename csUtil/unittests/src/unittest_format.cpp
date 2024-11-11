#include <format>
#include <iostream>
#include <print>

#include <catch2/catch_template_test_macros.hpp>

#include <cs/Core/Constants.h>
#include <cs/Core/Flags.h>

enum class FormatFlag : unsigned {
  None  = 0,
  Left  = 0x01,
  Upper = 0x02
};

CS_ENABLE_FLAGS(FormatFlag);

using FormatFlags = cs::Flags<FormatFlag>;

using size_type = std::size_t;

constexpr size_type STRING_LEN = 128;

namespace util {

  /*
   * Example (8bit):
   *
   *   signed min: .10000000
   *   signed max: ..1111111
   * unsigned min: ........0
   * unsigned max: .11111111
   */
  template<typename T>
  requires cs::is_integral_v<T>
  std::string make_binstring(const T value,
                             const size_type width, const char fill,
                             const FormatFlags flags = FormatFlag::None)
  {
    constexpr T MIN = cs::Konst<T>::MIN;
    constexpr T MAX = cs::Konst<T>::MAX;

    if( value != MIN  &&  value != MAX ) {
      return std::string{};
    }

    const bool is_left = flags.testAny(FormatFlag::Left);

    const bool is_maximum = value == MAX;
    const bool is_minimum = value == MIN;

    const size_type value_len = std::is_unsigned_v<T>  &&  is_minimum
        ? 1
        : std::is_signed_v<T>  &&  is_maximum
          ? sizeof(T)*8 - 1
          : sizeof(T)*8;

    std::string s;
    s.reserve(STRING_LEN);

    // Align Right ///////////////////////////////////////////////////////////

    if( !is_left ) {
      for(size_type i = value_len; i < width; i++) {
        s.push_back(fill);
      }
    }

    // First Character ///////////////////////////////////////////////////////

    if constexpr( std::is_unsigned_v<T> ) {
      if( is_minimum ) {
        s.push_back('0');
      } else {
        s.push_back('1');
      }
    } else {
      s.push_back('1');
    }

    // i-th Character ////////////////////////////////////////////////////////

    for(size_type i = 1; i < value_len; i++) {
      if( is_minimum ) {
        s.push_back('0');
      } else {
        s.push_back('1');
      }
    }

    // Align Left ////////////////////////////////////////////////////////////

    if( is_left ) {
      for(size_type i = value_len; i < width; i++) {
        s.push_back(fill);
      }
    }

    return s;
  }

  /*
   * Example (16bit):
   *
   *   signed min: .8000
   *   signed max: .7fff
   * unsigned min: ....0
   * unsigned max: .ffff
   */
  template<typename T>
  requires cs::is_integral_v<T>
  std::string make_hexstring(const T value,
                             const size_type width, const char fill,
                             const FormatFlags flags = FormatFlag::Upper)
  {
    constexpr T MIN = cs::Konst<T>::MIN;
    constexpr T MAX = cs::Konst<T>::MAX;

    if( value != MIN  &&  value != MAX ) {
      return std::string{};
    }

    const bool is_left = flags.testAny(FormatFlag::Left);

    const bool is_minimum = value == MIN;

    const char glyph_f = flags.testAny(FormatFlag::Upper)
        ? 'F'
        : 'f';

    const size_type value_len = std::is_unsigned_v<T>  &&  is_minimum
        ? 1
        : sizeof(T)*2;

    std::string s;
    s.reserve(STRING_LEN);

    // Align Right ///////////////////////////////////////////////////////////

    if( !is_left ) {
      for(size_type i = value_len; i < width; i++) {
        s.push_back(fill);
      }
    }

    // First Character ///////////////////////////////////////////////////////

    if constexpr( std::is_signed_v<T> ) {
      if( is_minimum ) {
        s.push_back('8');
      } else {
        s.push_back('7');
      }
    } else {
      if( is_minimum ) {
        s.push_back('0');
      } else {
        s.push_back(glyph_f);
      }
    }

    // i-th Character ////////////////////////////////////////////////////////

    for(size_type i = 1; i < value_len; i++) {
      if( is_minimum ) {
        s.push_back('0');
      } else {
        s.push_back(glyph_f);
      }
    }

    // Align Left ////////////////////////////////////////////////////////////

    if( is_left ) {
      for(size_type i = value_len; i < width; i++) {
        s.push_back(fill);
      }
    }

    return s;
  }

  using TwoStrings = std::pair<std::string,std::string>;

  template<bool OUTPUT = false>
  void print(const TwoStrings& p)
  {
    if constexpr( !OUTPUT ) {
      return;
    }
    std::println("{}  <->  {}", p.first, p.second);
  }

} // namespace util

namespace test_align {

  TEST_CASE("Alignment of text.", "[align]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr size_type WIDTH = 4;

    { // left
      REQUIRE( std::format("{0:.<{1}}", "x",     WIDTH)     == "x..."  );
      REQUIRE( std::format("{0:.<{1}}", "xxx",   WIDTH)   == "xxx."  );
      REQUIRE( std::format("{0:.<{1}}", "xxxx",  WIDTH)  == "xxxx"  );
      REQUIRE( std::format("{0:.<{1}}", "xxxxx", WIDTH) == "xxxxx" );
    }

    { // right
      REQUIRE( std::format("{0:.>{1}}", "x",     WIDTH) == "...x"  );
      REQUIRE( std::format("{0:.>{1}}", "xxx",   WIDTH) == ".xxx"  );
      REQUIRE( std::format("{0:.>{1}}", "xxxx",  WIDTH) == "xxxx"  );
      REQUIRE( std::format("{0:.>{1}}", "xxxxx", WIDTH) == "xxxxx" );
    }
  } // TEST_CASE

} // namespace test_align

namespace test_hex {

  template<typename T>
  requires cs::is_integral_v<T>
  util::TwoStrings make_hex(const T value, const bool fill_digits)
  {
    const size_type width = fill_digits
        ? sizeof(T)*2
        : 0;

    const std::string val_str = std::format("{0:0{1}X}", std::make_unsigned_t<T>(value), width);
    const std::string ref_str = util::make_hexstring(value, width, '0');

    return util::TwoStrings{val_str, ref_str};
  }

  TEMPLATE_TEST_CASE("Hexadecimal formatting of integral types.", "[hexf]",
                     int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    { // Minimum /////////////////////////////////////////////////////////////
      constexpr value_type MIN = cs::Konst<value_type>::MIN;

      {
        const util::TwoStrings p = make_hex(MIN, false);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const util::TwoStrings p = make_hex(MIN, true);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // Minimum

    { // Maximum /////////////////////////////////////////////////////////////
      constexpr value_type MAX = cs::Konst<value_type>::MAX;

      {
        const util::TwoStrings p = make_hex(MAX, false);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const util::TwoStrings p = make_hex(MAX, true);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // Maximum
  } // TEST_CASE

} // namespace test_hex

namespace test_bin {

  template<typename T>
  requires cs::is_integral_v<T>
  util::TwoStrings make_bin(const T value, const bool fill_digits)
  {
    const size_type width = fill_digits
        ? sizeof(T)*8
        : 0;

    const std::string val_str = std::format("{0:0{1}b}", std::make_unsigned_t<T>(value), width);
    const std::string ref_str = util::make_binstring(value, width, '0');

    return util::TwoStrings{val_str, ref_str};
  }

  TEMPLATE_TEST_CASE("Binary formatting of integral types.", "[binf]",
                     int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    { // Minimum /////////////////////////////////////////////////////////////
      constexpr value_type MIN = cs::Konst<value_type>::MIN;

      {
        const util::TwoStrings p = make_bin(MIN, false);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const util::TwoStrings p = make_bin(MIN, true);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // Minimum

    { // Maximum /////////////////////////////////////////////////////////////
      constexpr value_type MAX = cs::Konst<value_type>::MAX;

      {
        const util::TwoStrings p = make_bin(MAX, false);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const util::TwoStrings p = make_bin(MAX, true);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // Maximum
  } // TEST_CASE

} // namespace test_bin
