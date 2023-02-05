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
  template<typename T, bool IS_MINIMUM>
  requires cs::IsIntegral<T>
  std::string make_hexstring(const size_type width, const char fill,
                             const cs::FormatFlags& flags)
  {
    constexpr size_type STRING_LEN = 32;
    constexpr size_type  VALUE_LEN = std::is_unsigned_v<T>  &&  IS_MINIMUM
        ? 1
        : sizeof(T)*2;

    const bool is_left  = flags.testAny(cs::FormatFlag::Left);
    const bool is_upper = flags.testAny(cs::FormatFlag::Upper);

    std::string s;
    s.reserve(STRING_LEN);

    // Align Right ///////////////////////////////////////////////////////////

    if( !is_left ) {
      for(size_type i = VALUE_LEN; i < width; i++) {
        s.push_back(fill);
      }
    }

    // First Character ///////////////////////////////////////////////////////

    if constexpr( std::is_signed_v<T> ) {
      if constexpr( IS_MINIMUM ) {
        s.push_back('8');
      } else {
        s.push_back('7');
      }
    } else {
      if constexpr( IS_MINIMUM ) {
        s.push_back('0');
      } else {
        if( is_upper ) {
          s.push_back('F');
        } else {
          s.push_back('f');
        }
      }
    }

    // i-th Character ////////////////////////////////////////////////////////

    for(size_type i = 1; i < VALUE_LEN; i++) {
      if constexpr( IS_MINIMUM ) {
        s.push_back('0');
      } else {
        if( is_upper ) {
          s.push_back('F');
        } else {
          s.push_back('f');
        }
      }
    }

    // Align Left ////////////////////////////////////////////////////////////

    if( is_left ) {
      for(size_type i = VALUE_LEN; i < width; i++) {
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

  template<typename T, bool IS_MINIMUM>
  requires cs::IsIntegral<T>
  util::TwoStrings make_integral(const cs::FormatFlags& flags)
  {
    constexpr size_type WIDTH = 19;
    constexpr      char  FILL = '.';

    const T value = IS_MINIMUM
        ? std::numeric_limits<T>::lowest()
        : std::numeric_limits<T>::max();

    const std::string val_str =
        cs::sprint("%", cs::format(value, 16, WIDTH, FILL, flags));
    const std::string ref_str =
        util::make_hexstring<T,IS_MINIMUM>(WIDTH, FILL, flags);

    return util::TwoStrings{val_str, ref_str};
  }

  TEMPLATE_TEST_CASE("Formatting of integral types.", "[integral]",
                     int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t) {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    using value_type = TestType;

    { // IS_MINIMUM == true //////////////////////////////////////////////////
      constexpr bool IS_MINIMUM = true;

      {
        const cs::FormatFlags flags{cs::FormatFlag::None};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left | cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // IS_MINIMUM

    { // IS_MINIMUM == false /////////////////////////////////////////////////
      constexpr bool IS_MINIMUM = false;

      {
        const cs::FormatFlags flags{cs::FormatFlag::None};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }

      {
        const cs::FormatFlags flags{cs::FormatFlag::Left | cs::FormatFlag::Upper};
        const util::TwoStrings p = make_integral<value_type,IS_MINIMUM>(flags);
        util::print(p);
        REQUIRE( p.first == p.second );
      }
    } // IS_MINIMUM
  } // TEST_CASE

} // namespace test_integral
