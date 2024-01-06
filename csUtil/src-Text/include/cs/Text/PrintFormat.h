/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#pragma once

#include <array>
#include <charconv>
#include <ostream>

#include <cs/Core/Flags.h>
#include <cs/Text/StringAlgorithm.h>

namespace cs {

  enum class FormatFlag : unsigned int {
    None  = 0,
    Left  = 0x01, // default: Right
    Upper = 0x02
  };

} // namespace cs

CS_ENABLE_FLAGS(cs::FormatFlag);

namespace cs {

  using FormatFlags = Flags<FormatFlag>;

  namespace impl_format {

    ////// Implementation ////////////////////////////////////////////////////

    inline constexpr std::size_t BUF_SIZE = 128;

    template<typename CharT>
    requires is_char_v<CharT>
    inline void output(std::basic_ostream<CharT> *stream,
                       const char *str, const std::size_t lenstr,
                       const std::size_t width, const CharT fill,
                       const FormatFlags& flags)
    {
      const bool is_left = flags.testAny(FormatFlag::Left);

      if( !is_left ) { // right adjust -> fill left
        for(std::size_t i = lenstr; i < width; i++) {
          stream->put(fill);
        }
      }

      if constexpr( is_widechar_v<CharT> ) {
        std::array<CharT,BUF_SIZE> wdata;
        const std::size_t numWiden = widen_s(wdata.data(), wdata.size(), str, lenstr);
        stream->write(wdata.data(), numWiden);
      } else {
        stream->write(str, lenstr);
      }

      if( is_left ) { // left adjust -> fill right
        for(std::size_t i = lenstr; i < width; i++) {
          stream->put(fill);
        }
      }
    }

    ////// Integral Types ////////////////////////////////////////////////////

    template<typename T, typename CharT>
    requires is_integral_v<T>  &&  is_char_v<CharT>
    class FormatIntegral {
    public:
      using stream_type = std::basic_ostream<CharT>;
      using   char_type = typename stream_type::char_type;
      using   size_type = std::size_t;
      using  value_type = T;

      FormatIntegral(const value_type value,
                     const int base,
                     const size_type width, const char_type fill,
                     const FormatFlags& flags) noexcept
        : _value{value}
        , _base{base}
        , _width{width}
        , _fill{fill}
        , _flags{flags}
      {
      }

      ~FormatIntegral() noexcept = default;

      stream_type& operator()(stream_type *stream) const
      {
        // (1) Convert number to string //////////////////////////////////////

        std::array<char,BUF_SIZE> ndata;
        const std::to_chars_result result =
            std::to_chars(ndata.data(), ndata.data() + ndata.size(),
                          _value, _base);
        if( result.ec != std::errc{} ) {
          return *stream << "ERROR";
        }

        // (2) (Optionally) Convert string to upper case /////////////////////

        if( _flags.testAny(FormatFlag::Upper) ) {
          toUpper_s(ndata.data(), result.ptr);
        }

        // (3) Get string's bounds ///////////////////////////////////////////

        const char *str = _base != 10  &&  ndata[0] == '-'
            ? ndata.data() + 1
            : ndata.data();
        const size_type lenstr = strlen(str, result.ptr);

        // (4) Output string /////////////////////////////////////////////////

        output(stream, str, lenstr, _width, _fill, _flags);

        return *stream;
      }

    private:
      FormatIntegral() noexcept = delete;

      const value_type  _value{};
      const int          _base{};
      const size_type   _width{};
      const char_type    _fill{};
      const FormatFlags _flags{};
    };

    ////// Real Types ////////////////////////////////////////////////////////

    template<typename T, typename CharT>
    requires is_real_v<T>  &&  is_char_v<CharT>
    class FormatReal {
    public:
      using stream_type = std::basic_ostream<CharT>;
      using   char_type = typename stream_type::char_type;
      using   size_type = std::size_t;
      using  value_type = T;

      FormatReal(const value_type value,
                 const std::chars_format format, const int precision,
                 const size_type width, const char_type fill,
                 const FormatFlags& flags) noexcept
        : _value{value}
        , _format{format}
        , _precision{precision}
        , _width{width}
        , _fill{fill}
        , _flags{flags}
      {
      }

      ~FormatReal() noexcept = default;

      stream_type& operator()(stream_type *stream) const
      {
        // (1) Convert number to string //////////////////////////////////////

        std::array<char,BUF_SIZE> ndata;
        const std::to_chars_result result =
            std::to_chars(ndata.data(), ndata.data() + ndata.size(),
                          _value, _format, _precision);
        if( result.ec != std::errc{} ) {
          return *stream << "ERROR";
        }

        // (2) (Optionally) Convert string to upper case /////////////////////

        if( _flags.testAny(FormatFlag::Upper) ) {
          toUpper_s(ndata.data(), result.ptr);
        }

        // (3) Get string's bounds ///////////////////////////////////////////

        const char *str = ndata.data();
        const std::size_t lenstr = strlen(str, result.ptr);

        // (4) Output string /////////////////////////////////////////////////

        output(stream, str, lenstr, _width, _fill, _flags);

        return *stream;
      }

    private:
      FormatReal() noexcept = delete;

      const value_type         _value{};
      const std::chars_format _format{};
      const int            _precision{};
      const size_type          _width{};
      const char_type           _fill{};
      const FormatFlags        _flags{};
    };

  } // namespace impl_format

  ////// STD Stream Interface ////////////////////////////////////////////////

  template<typename CharT, typename T>
  inline std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& stream,
                                               const cs::impl_format::FormatIntegral<T,CharT>& formatter)
  {
    return formatter(&stream);
  }

  template<typename CharT, typename T>
  inline std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& stream,
                                               const cs::impl_format::FormatReal<T,CharT>& formatter)
  {
    return formatter(&stream);
  }

  ////// User Interface - Integral ///////////////////////////////////////////

  inline constexpr std::size_t DEFINT_WIDTH = 0;
  template<typename CharT> requires is_char_v<CharT>
  inline constexpr CharT       DEFINT_FILL  = glyph<CharT>::space;
  inline constexpr FormatFlag  DEFINT_FLAGS = FormatFlag::None;

  template<typename T, typename CharT = char>
  requires is_integral_v<T>  &&  is_char_v<CharT>
  inline auto format(const T value, const int base,
                     const std::size_t width = DEFINT_WIDTH,
                     const CharT fill = DEFINT_FILL<CharT>,
                     const FormatFlags& flags = DEFINT_FLAGS)
  {
    return impl_format::FormatIntegral<T,CharT>{value, base, width, fill, flags};
  }

  template<typename T, typename CharT = char>
  requires is_integral_v<T>  &&  is_char_v<CharT>
  inline auto binf(const T value, const bool fill_digits = false)
  {
    const std::size_t width = fill_digits
        ? sizeof(T)*8
        : 0;
    return format<T,CharT>(value, 2, width, glyph<CharT>::zero);
  }

  template<typename T, typename CharT = char>
  requires is_integral_v<T>  &&  is_char_v<CharT>
  inline auto decf(const T value, const std::size_t width, const CharT fill = DEFINT_FILL<CharT>)
  {
    return format<T,CharT>(value, 10, width, fill);
  }

  template<typename T, typename CharT = char>
  requires is_integral_v<T>  &&  is_char_v<CharT>
  inline auto hexf(const T value, const bool fill_digits = false)
  {
    const std::size_t width = fill_digits
        ? sizeof(T)*2
        : 0;
    return format<T,CharT>(value, 16, width, glyph<CharT>::zero, FormatFlag::Upper);
  }

  ////// User Interface - Real ///////////////////////////////////////////////

  inline constexpr int         DEFREAL_PRECISION = 6;
  inline constexpr std::size_t DEFREAL_WIDTH     = 0;
  template<typename CharT> requires is_char_v<CharT>
  inline constexpr CharT       DEFREAL_FILL      = glyph<CharT>::space;
  inline constexpr FormatFlag  DEFREAL_FLAGS     = FormatFlag::None;

  template<typename T, typename CharT = char>
  requires is_real_v<T>  &&  is_char_v<CharT>
  inline auto format(const T value, const std::chars_format format,
                     const int precision = DEFREAL_PRECISION,
                     const std::size_t width = DEFREAL_WIDTH,
                     const CharT fill = DEFREAL_FILL<CharT>,
                     const FormatFlags& flags = DEFREAL_FLAGS)
  {
    return impl_format::FormatReal<T,CharT>{value, format, precision, width, fill, flags};
  }

  template<typename T, typename CharT = char>
  requires is_real_v<T>  &&  is_char_v<CharT>
  inline auto fixedf(const T value, const int precision = DEFREAL_PRECISION)
  {
    return format<T,CharT>(value, std::chars_format::fixed, precision);
  }

  template<typename T, typename CharT = char>
  requires is_real_v<T>  &&  is_char_v<CharT>
  inline auto generalf(const T value, const int precision = DEFREAL_PRECISION)
  {
    return format<T,CharT>(value, std::chars_format::general, precision);
  }

  template<typename T, typename CharT = char>
  requires is_real_v<T>  &&  is_char_v<CharT>
  inline auto hexf(const T value, const int precision = DEFREAL_PRECISION)
  {
    return format<T,CharT>(value, std::chars_format::hex, precision);
  }

  template<typename T, typename CharT = char>
  requires is_real_v<T>  &&  is_char_v<CharT>
  inline auto scientificf(const T value, const int precision = DEFREAL_PRECISION)
  {
    return format<T,CharT>(value, std::chars_format::scientific, precision);
  }

} // namespace cs
