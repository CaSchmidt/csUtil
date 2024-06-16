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

#include <cs/Text/StringUtil.h>
#include <cs/Text/StringValue.h>

namespace cs {

  template<typename CharT> requires is_char_v<CharT>
  inline std::basic_string<CharT> left(std::basic_string<CharT> str,
                                       const std::size_t width,
                                       const CharT fill = glyph<CharT>::space)
  {
    if( str.empty()  ||  str.size() >= width ) {
      return str;
    }

    try {
      str.append(width - str.size(), fill);
    } catch( ... ) {
      return std::basic_string<CharT>();
    }

    return str;
  }

  inline std::string left(std::string str,
                          const std::size_t width,
                          const char fill = ' ')
  {
    return left<char>(str, width, fill);
  }

  template<typename CharT> requires is_char_v<CharT>
  inline std::basic_string<CharT> right(std::basic_string<CharT> str,
                                        const std::size_t width,
                                        const CharT fill = glyph<CharT>::space)
  {
    if( str.empty()  ||  str.size() >= width ) {
      return str;
    }

    try {
      str.insert(0, width - str.size(), fill);
    } catch( ... ) {
      return std::basic_string<CharT>();
    }

    return str;
  }

  inline std::string right(std::string str,
                           const std::size_t width,
                           const char fill = ' ')
  {
    return right<char>(str, width, fill);
  }

  template<typename T, bool RECLAIM = true>
  inline if_integral_t<T,std::string> binf(const T value,
                                           const bool fill_digits = false)
  {
    const std::size_t width = fill_digits
        ? sizeof(T)*8
        : 0;

    std::string str = right(toString<T,false>(value, 2), width, '0');

    if constexpr( RECLAIM ) {
      impl_strval::shrink(str, RECLAIM);
    }

    return str;
  }

  template<typename T, bool RECLAIM = true>
  inline if_integral_t<T,std::string> decf(const T value,
                                           const std::size_t width,
                                           const char fill = ' ')
  {
    std::string str = right(toString<T,false>(value, 10), width, fill);

    if constexpr( RECLAIM ) {
      impl_strval::shrink(str, RECLAIM);
    }

    return str;
  }

  template<typename T, bool RECLAIM = true>
  inline if_integral_t<T,std::string> hexf(const T value,
                                           const bool fill_digits = false)
  {
    const std::size_t width = fill_digits
        ? sizeof(T)*2
        : 0;

    std::string str = right(toString<T,false>(value, 16), width, '0');

    upper(str);

    if constexpr( RECLAIM ) {
      impl_strval::shrink(str, RECLAIM);
    }

    return str;
  }

} // namespace cs
