#include <iostream>
#include <limits>

#include <catch.hpp>

#include <cs/Core/PrintFormat.h>
#include <cs/Core/PrintUtil.h>

using size_type = std::size_t;

namespace util {

  /*
   * Example (16bit):
   *
   *   signed min: .8000
   *   signed max: .7fff
   * unsigned min: ....0
   * unsigned max: .ffff
   */
  template<typename T>
  requires cs::IsIntegral<T>
  std::string make_hexstring(const T value,
                             const size_type width, const char fill,
                             const cs::FormatFlags& flags)
  {
    constexpr T MIN = std::numeric_limits<T>::lowest();
    constexpr T MAX = std::numeric_limits<T>::max();

    constexpr size_type STRING_LEN = 32;

    if( value != MIN  &&  value != MAX ) {
      return std::string{};
    }

    const bool is_left    = flags.testAny(cs::FormatFlag::Left);
    const bool is_minimum = value == MIN;

    const char glyph_f = flags.testAny(cs::FormatFlag::Upper)
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
    cs::println("%  <->  %", p.first, p.second);
  }

} // namespace util

namespace test_integral {

  template<typename T>
  requires cs::IsIntegral<T>
  util::TwoStrings make_integral(const T value, const cs::FormatFlags& flags)
  {
    constexpr size_type WIDTH = 19;
    constexpr      char  FILL = '.';

    const std::string val_str =
        cs::sprint("%", cs::format(value, 16, WIDTH, FILL, flags));
    const std::string ref_str =
        util::make_hexstring<T>(value, WIDTH, FILL, flags);

    return util::TwoStrings{val_str, ref_str};
  }

  TEMPLATE_TEST_CASE("Formatting of integral types.", "[integral]",
                     int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    { // Minimum /////////////////////////////////////////////////////////////
      const value_type MIN = std::numeric_limits<value_type>::lowest();

      {
        const cs::FormatFlags flags{cs::FormatFlag::None};
        const util::TwoStrings p = make_integral(MIN, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left};
        const util::TwoStrings p = make_integral(MIN, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral(MIN, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left | cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral(MIN, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // Minimum

    { // Maximum /////////////////////////////////////////////////////////////
      const value_type MAX = std::numeric_limits<value_type>::max();

      {
        const cs::FormatFlags flags{cs::FormatFlag::None};
        const util::TwoStrings p = make_integral(MAX, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left};
        const util::TwoStrings p = make_integral(MAX, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral(MAX, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left | cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral(MAX, flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // Maximum
  } // TEST_CASE

} // namespace test_integral
