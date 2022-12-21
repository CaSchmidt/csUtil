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

#include <cs/Core/StringAlgorithm.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_stream {

    inline constexpr std::size_t BUF_SIZE = 128;

    template<typename CharT>
    requires IsCharacter<CharT>
    inline void output(std::basic_ostream<CharT> *stream,
                       const char *str, const std::size_t lenstr)
    {
      if constexpr( is_widechar_v<CharT> ) {
        std::array<CharT,BUF_SIZE> wdata;
        widen(wdata.data(), str, lenstr);
        stream->write(wdata.data(), lenstr);
      } else {
        stream->write(str, lenstr);
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
                   const char format,
                   const size_type width, const char_type fill) noexcept
      : _value{value}
      , _format{format}
      , _width{width}
      , _fill{fill}
    {
    }

    ~FormatIntegral() noexcept = default;

    stream_type& operator()(stream_type *stream) const
    {
      const int base = FormatIntegral::base();

      // (1) Convert number to string ////////////////////////////////////////

      std::array<char,impl_stream::BUF_SIZE> ndata;
      const std::to_chars_result result =
          std::to_chars(ndata.data(), ndata.data() + ndata.size(),
                        _value, base);
      if( result.ec != std::errc{} ) {
        return *stream << "ERROR";
      }

      // (2) (Optionally) Convert string to upper case ///////////////////////

      if( isUpper() ) {
        toUpper(ndata.data(), result.ptr);
      }

      // (3) Get string's bounds /////////////////////////////////////////////

      const char *str = base != 10  &&  ndata[0] == '-'
          ? ndata.data() + 1
          : ndata.data();
      const size_type lenstr = distance(str, result.ptr);

      // (4) Fill field to width /////////////////////////////////////////////

      for(size_type i = lenstr; i < _width; i++) {
        stream->put(_fill);
      }

      // (5) Output string ///////////////////////////////////////////////////

      impl_stream::output(stream, str, lenstr);

      return *stream;
    }

  private:
    FormatIntegral() noexcept = delete;

    int base() const
    {
      if(        _format == 'b' ) {
        return 2;
      } else if( _format == 'o' ) {
        return 8;
      } else if( _format == 'x'  ||  _format == 'X' ) {
        return 16;
      }
      return 10; // 'd'
    }

    bool isUpper() const
    {
      return _format == 'X';
    }

    const value_type _value{};
    const char      _format{};
    const size_type  _width{};
    const char_type   _fill{};
  };

  ////// Real Types //////////////////////////////////////////////////////////

  ////// STD Stream Interface ////////////////////////////////////////////////

  template<typename CharT, typename T>
  inline std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& stream,
                                               const cs::FormatIntegral<T,CharT>& formatter)
  {
    return formatter(&stream);
  }

  ////// User Interface //////////////////////////////////////////////////////

  inline constexpr std::size_t DEFINT_WIDTH = 0;
  template<typename CharT>
  requires IsCharacter<CharT>
  inline constexpr CharT DEFINT_FILL = glyph<CharT>::space;

  template<typename T>
  requires IsIntegral<T>
  inline FormatIntegral<T,char> format(const T value,
                                       const char format,
                                       const std::size_t width = DEFINT_WIDTH,
                                       const char fill = DEFINT_FILL<char>)
  {
    return FormatIntegral<T,char>{value, format, width, fill};
  }

  template<typename T>
  requires IsIntegral<T>
  inline FormatIntegral<T,wchar_t> wformat(const T value,
                                           const char format,
                                           const std::size_t width = DEFINT_WIDTH,
                                           const wchar_t fill = DEFINT_FILL<wchar_t>)
  {
    return FormatIntegral<T,wchar_t>{value, format, width, fill};
  }

} // namespace cs
