#include <cstdio>
#include <cstdlib>

#include <array>
#include <bitset>
#include <charconv>
#include <iostream>
#include <limits>

#include <cs/Core/Print.h>
#include <cs/Core/PrintFormat.h>

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

  } // namespace impl_stream

} // namespace cs

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
  cs::println(std::wcout, L"PI = %", cs::wformat(3.1415, 'g'));

  return EXIT_SUCCESS;
}
