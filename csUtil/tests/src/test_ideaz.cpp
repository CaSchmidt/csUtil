#include <cstdio>
#include <cstdlib>

#include <array>
#include <bitset>
#include <charconv>
#include <iostream>
#include <limits>

#include <cs/Core/Print.h>
#include <cs/Core/PrintFormat.h>

#define USE_TO_CHARS

namespace cs {

  namespace impl_stream {

    template<typename CharT>
    requires IsCharacter<CharT>
    struct Context {
      using stream_type = std::basic_ostream<CharT>;
      using   char_type = typename stream_type::char_type;

      Context() noexcept = default;

      Context(const stream_type& stream) noexcept
      {
        save(stream);
      }

      void restore(stream_type *stream) const
      {
        stream->fill(fill);
        stream->flags(flags);
        stream->precision(precision);
        stream->width(width);
      }

      void save(const stream_type& stream)
      {
        fill      = stream.fill();
        flags     = stream.flags();
        precision = stream.precision();
        width     = stream.width();
      }

      char_type                fill{};
      std::ios_base::fmtflags flags{};
      std::streamsize     precision{};
      std::streamsize         width{};
    };

    template<typename CharT>
    requires IsCharacter<CharT>
    inline std::streamsize distance(const CharT *first, const CharT *last)
    {
      return first != nullptr  &&  first < last
          ? std::streamsize(last - first)
          : 0;
    }

  } // namespace impl_stream

  ////// Formatting of Real Values ///////////////////////////////////////////

  template<typename CharT>
  requires IsCharacter<CharT>
  inline constexpr CharT           DEFREAL_FILL = glyph<CharT>::space;
  inline constexpr char            DEFREAL_FORMAT = 'g';
  inline constexpr std::streamsize DEFREAL_PRECISION = 6;
  inline constexpr std::streamsize DEFREAL_WIDTH = 0;

  template<typename T, typename CharT>
  requires IsReal<T>  &&  IsCharacter<CharT>
  class FormatReal {
  public:
    using stream_type = std::basic_ostream<CharT>;
    using   char_type = typename stream_type::char_type;
    using  value_type = T;

    FormatReal(const value_type value,
               const char format = DEFREAL_FORMAT, // format := [aAeEfFgG]
               const std::streamsize precision = DEFREAL_PRECISION,
               const std::streamsize width = DEFREAL_WIDTH,
               const char_type fill = DEFREAL_FILL<char_type>) noexcept
      : _fill{fill}
      , _format{format}
      , _precision{precision}
      , _value{value}
      , _width{width}
    {
    }

    ~FormatReal() noexcept = default;

    inline stream_type& operator()(stream_type *stream) const
    {
#ifdef USE_TO_CHARS
      std::array<char,128> strdata;
      const std::to_chars_result result =
          std::to_chars(strdata.data(), strdata.data() + strdata.size(),
                        _value, format(), int(_precision));
      if( result.ec != std::errc{} ) {
        return *stream << "ERROR";
      }

      if( isUpper() ) {
        toUpper(strdata.data(), result.ptr);
      }

      const char *str = strdata.data();
      const std::streamsize strlen = impl_stream::distance(str, result.ptr);

      for(std::streamsize i = strlen; i < _width; i++) {
        stream->put(_fill);
      }
      stream->write(str, strlen);

      return *stream;
#else
      const impl_stream::Context<char_type> ctx{*stream};

      if(        _format == 'a'  ||  _format == 'A' ) {
        stream->flags(std::ios_base::fixed | std::ios_base::scientific);
      } else if( _format == 'e'  ||  _format == 'E' ) {
        stream->flags(std::ios_base::scientific);
      } else if( _format == 'f'  ||  _format == 'F' ) {
        stream->flags(std::ios_base::fixed);
      } else { // 'g'  ||  'G'
        stream->flags(std::ios_base::fmtflags{});
      }
      // NOTE: Use setf() from hereon to ADD flags!
      if( _format == 'A'  ||  _format == 'E'  ||  _format == 'F'  ||  _format == 'G' ) {
        stream->setf(std::ios_base::uppercase);
      }
      stream->precision(_precision);
      stream->width(_width);
      stream->fill(_fill);

      *stream << _value;

      ctx.restore(stream);

      return *stream;
#endif
    }

  private:
    FormatReal() noexcept = delete;

    inline std::chars_format format() const
    {
      if(        _format == 'a'  ||  _format == 'A' ) {
        return std::chars_format::hex;
      } else if( _format == 'e'  ||  _format == 'E' ) {
        return std::chars_format::scientific;
      } else if( _format == 'f'  ||  _format == 'F' ) {
        return std::chars_format::fixed;
      }
      return std::chars_format::general;
    }

    inline bool isUpper() const
    {
      return
          _format == 'A'  ||
          _format == 'E'  ||
          _format == 'F'  ||
          _format == 'G';
    }

    const char_type            _fill{};
    const char               _format{};
    const std::streamsize _precision{};
    const value_type          _value{};
    const std::streamsize     _width{};
  };

  ////// User Interface //////////////////////////////////////////////////////

  template<typename T>
  requires IsReal<T>
  inline FormatReal<T,char> format(const T value,
                                   const char format = DEFREAL_FORMAT,
                                   const std::streamsize precision = DEFREAL_PRECISION,
                                   const std::streamsize width = DEFREAL_WIDTH,
                                   const char fill = DEFREAL_FILL<char>)
  {
    return FormatReal<T,char>{value, format, precision, width, fill};
  }

} // namespace cs

template<typename CharT, typename T>
inline std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& stream,
                                             const cs::FormatReal<T,CharT>& formatter)
{
  return formatter(&stream);
}

/* ************************************************************************ */

/* ************************************************************************ */

template<typename T>
void test_int(const T value,
              const std::streamsize width = 10)
{
  std::cout << sizeof(T)*8 << "bit, "
            << (std::is_unsigned_v<T> ? "unsigned" : "signed")
            << std::endl;
  std::cout << cs::format(value, 'b', width, '.') << std::endl;
  std::cout << cs::format(value, 'd', width, '.') << std::endl;
  std::cout << cs::format(value, 'o', width, '.') << std::endl;
  std::cout << cs::format(value, 'x', width, '.') << std::endl;
  std::cout << cs::format(value, 'X', width, '.') << std::endl;
  std::cout << "*******************************" << std::endl;
}

template<typename T>
void test_real(const T value,
               const std::streamsize prec = 6,
               const std::streamsize width = 10)
{
  std::cout << sizeof(T)*8 << "bit" << std::endl;
  std::cout << cs::format(value, 'a', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'A', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'e', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'E', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'f', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'F', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'g', prec, width, '.') << std::endl;
  std::cout << cs::format(value, 'G', prec, width, '.') << std::endl;
  std::cout << "*******************************" << std::endl;
}

int main(int, char **)
{
  test_int(std::numeric_limits<int8_t>::lowest());
  test_int(std::numeric_limits<int8_t>::max());
  test_int(std::numeric_limits<uint8_t>::lowest());
  test_int(std::numeric_limits<uint8_t>::max());
  test_real(1.0/32.0);

  cs::println("We are at %% efficiency!", 100, '%');
  cs::println("value = 0x%", cs::format(123, 'X', 8, '0'));
  cs::println(std::wcout, L"value = 0x%", cs::wformat(123, 'X', 8, '0'));

  return EXIT_SUCCESS;
}
