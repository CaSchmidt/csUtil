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
#include <cs/Core/StringAlgorithm.h>

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

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_stream {

    inline constexpr std::size_t BUF_SIZE = 128;

    template<typename CharT>
    requires IsCharacter<CharT>
    inline void output(std::basic_ostream<CharT> *stream,
                       const char *str, const std::size_t lenstr,
                       const std::size_t width, const CharT fill,
                       const FormatFlags& flags)
    {
      if( !flags.testAny(FormatFlag::Left) ) { // right adjust -> fill left
        for(std::size_t i = lenstr; i < width; i++) {
          stream->put(fill);
        }
      }

      if constexpr( is_widechar_v<CharT> ) {
        std::array<CharT,BUF_SIZE> wdata;
        widen(wdata.data(), str, lenstr);
        stream->write(wdata.data(), lenstr);
      } else {
        stream->write(str, lenstr);
      }

      if( flags.testAny(FormatFlag::Left) ) { // left adjust -> fill right
        for(std::size_t i = lenstr; i < width; i++) {
          stream->put(fill);
        }
      }
    }

  } // namespace impl_stream

  ////// Integral Types //////////////////////////////////////////////////////

  template<typename T, typename CharT>
  requires IsIntegral<T>  &&  IsCharacter<CharT>
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
      // (1) Convert number to string ////////////////////////////////////////

      std::array<char,impl_stream::BUF_SIZE> ndata;
      const std::to_chars_result result =
          std::to_chars(ndata.data(), ndata.data() + ndata.size(),
                        _value, _base);
      if( result.ec != std::errc{} ) {
        return *stream << "ERROR";
      }

      // (2) (Optionally) Convert string to upper case ///////////////////////

      if( _flags.testAny(FormatFlag::Upper) ) {
        toUpper(ndata.data(), result.ptr);
      }

      // (3) Get string's bounds /////////////////////////////////////////////

      const char *str = _base != 10  &&  ndata[0] == '-'
          ? ndata.data() + 1
          : ndata.data();
      const size_type lenstr = distance(str, result.ptr);

      // (4) Output string ///////////////////////////////////////////////////

      impl_stream::output(stream, str, lenstr, _width, _fill, _flags);

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

  ////// Real Types //////////////////////////////////////////////////////////

  template<typename T, typename CharT>
  requires IsReal<T>  &&  IsCharacter<CharT>
  class FormatReal {
  public:
    using stream_type = std::basic_ostream<CharT>;
    using   char_type = typename stream_type::char_type;
    using   size_type = std::size_t;
    using  value_type = T;

    FormatReal(const value_type value,
               const char format, const size_type precision,
               const size_type width, const char_type fill) noexcept
      : _value{value}
      , _format{format}
      , _precision{precision}
      , _width{width}
      , _fill{fill}
    {
    }

    ~FormatReal() noexcept = default;

    stream_type& operator()(stream_type *stream) const
    {
      // (1) Convert number to string ////////////////////////////////////////

      std::array<char,impl_stream::BUF_SIZE> ndata;
      const std::to_chars_result result =
          std::to_chars(ndata.data(), ndata.data() + ndata.size(),
                        _value, format(), int(_precision));
      if( result.ec != std::errc{} ) {
        return *stream << "ERROR";
      }

      // (2) (Optionally) Convert string to upper case ///////////////////////

      if( isUpper() ) {
        toUpper(ndata.data(), result.ptr);
      }

      // (3) Get string's bounds /////////////////////////////////////////////

      const char *str = ndata.data();
      const std::size_t lenstr = distance(str, result.ptr);

      // (4) Output string ///////////////////////////////////////////////////

      impl_stream::output(stream, str, lenstr, _width, _fill);

      return *stream;
    }

  private:
    FormatReal() noexcept = delete;

    std::chars_format format() const
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

    bool isUpper() const
    {
      return
          _format == 'A'  ||
          _format == 'E'  ||
          _format == 'F'  ||
          _format == 'G';
    }

    const value_type    _value{};
    const char         _format{};
    const size_type _precision{};
    const size_type     _width{};
    const char_type      _fill{};
  };

  ////// STD Stream Interface ////////////////////////////////////////////////

  template<typename CharT, typename T>
  inline std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& stream,
                                               const cs::FormatIntegral<T,CharT>& formatter)
  {
    return formatter(&stream);
  }

  template<typename CharT, typename T>
  inline std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& stream,
                                               const cs::FormatReal<T,CharT>& formatter)
  {
    return formatter(&stream);
  }

  ////// User Interface - Integral ///////////////////////////////////////////

  inline constexpr std::size_t DEFINT_WIDTH = 0;
  template<typename CharT>
  requires IsCharacter<CharT>
  inline constexpr CharT       DEFINT_FILL  = glyph<CharT>::space;
  inline constexpr FormatFlag  DEFINT_FLAGS = FormatFlag::None;

  template<typename T, typename CharT = char>
  requires IsIntegral<T>  &&  IsCharacter<CharT>
  inline FormatIntegral<T,CharT> format(const T value,
                                        const int base,
                                        const std::size_t width = DEFINT_WIDTH,
                                        const CharT fill = DEFINT_FILL<CharT>,
                                        const FormatFlags& flags = DEFINT_FLAGS)
  {
    return FormatIntegral<T,CharT>{value, base, width, fill, flags};
  }

  ////// User Interface - Real ///////////////////////////////////////////////

  inline constexpr std::size_t DEFREAL_PRECISION = 6;
  inline constexpr std::size_t DEFREAL_WIDTH = 0;
  template<typename CharT>
  requires IsCharacter<CharT>
  inline constexpr CharT DEFREAL_FILL = glyph<CharT>::space;

  template<typename T>
  requires IsReal<T>
  inline FormatReal<T,char> format(const T value,
                                   const char format,
                                   const std::size_t precision = DEFREAL_PRECISION,
                                   const std::size_t width = DEFREAL_WIDTH,
                                   const char fill = DEFREAL_FILL<char>)
  {
    return FormatReal<T,char>{value, format, precision, width, fill};
  }

} // namespace cs
