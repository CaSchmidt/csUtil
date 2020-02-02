#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <iomanip>
#include <ostream>
#include <type_traits>

template<typename CharT, typename T>
class FormatInt {
public:
  FormatInt(const std::enable_if_t<std::is_integral<T>::value,T>& value,
            const int   width = 1,
            const int    base = 10,
            const CharT& fill = static_cast<CharT>(' ')) noexcept
    : _base{base}
    , _fill{fill}
    , _value{value}
    , _width{width}
  {
  }

  ~FormatInt() noexcept = default;

  constexpr const int& base() const
  {
    return _base;
  }

  constexpr const CharT& fill() const
  {
    return _fill;
  }

  constexpr const T& value() const
  {
    return _value;
  }

  constexpr const int& width() const
  {
    return _width;
  }

private:
  FormatInt() noexcept = delete;

  FormatInt(const FormatInt&) noexcept = delete;
  FormatInt& operator=(const FormatInt&) noexcept = delete;

  FormatInt(FormatInt&&) noexcept = delete;
  FormatInt& operator=(FormatInt&&) noexcept = delete;

  const int   _base{};
  const CharT _fill{};
  const T     _value{};
  const int   _width{};
};

template<typename CharT, typename T, class Traits>
constexpr std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>& os,
                                                       const FormatInt<CharT,T>& fmt)
{
  return ( os
           << std::setfill(fmt.fill())
           << std::setw(fmt.width())
           << std::setbase(fmt.base())
           << fmt.value() );
}

#endif // TEST_UTIL_H
