/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <algorithm>
#include <array>
#include <charconv>
#include <string_view>
#include <tuple>

#include <cs/Core/CharUtil.h>
#include <cs/Lexer/LexerConfig.h>

namespace cs {

  namespace impl_lexer {

    struct ConvWrapper {
      using StringView = std::basic_string_view<LexerConfig::char_type>;
      using  size_type = StringView::size_type;
      using  char_type = StringView::value_type;

      using BasePrefix = std::tuple<int,size_type>; // [base, prefix]

      // NOTE: 'Buffer' and 'Range' MUST reference 'char'!
      using Buffer = std::array<char,128>;
      using Range  = std::tuple<const char*,const char*>;

      template<typename T>
      using Result = std::tuple<T,size_type>; // [value, consumed]

      template<typename T>
      inline static if_integral_t<T,Result<T>> toValue(const StringView& input,
                                                       const bool have_prefix = false)
      {
        const auto [base, prefix] = scanBasePrefix(input, have_prefix);
        if( base == 0 ) {
          return Result<T>{0, 0};
        }

        Buffer buffer;
        const auto [first, last] = makeRange(buffer, input, prefix);

        T value{0};
        const std::from_chars_result result =
            std::from_chars(first, last, value, base);
        if( result.ec != std::errc{} ) {
          return Result<T>{0, 0};
        }

        const size_type consumed = prefix + static_cast<size_type>(result.ptr - first);

        return Result<T>{value, consumed};
      }

      template<typename T>
      inline static if_real_t<T,Result<T>> toValue(const StringView& input)
      {
        if( !isRealPrefix(input) ) {
          return Result<T>{0, 0};
        }

        Buffer buffer;
        const auto [first, last] = makeRange(buffer, input, 0);

        T value{0};
        const std::from_chars_result result =
            std::from_chars(first, last, value, std::chars_format::general);
        if( result.ec != std::errc{} ) {
          return Result<T>{0, 0};
        }

        const size_type consumed = static_cast<size_type>(result.ptr - first);

        return Result<T>{value, consumed};
      }

    private:
      static_assert( std::is_unsigned_v<size_type> );

      using g = glyph<char_type>;

      inline static size_type fillBuffer(Buffer& buffer,
                                         const StringView& input,
                                         const size_type prefix)
      {
        const size_type length = maxLength(input, prefix);

        const char_type *src = input.data() + prefix;
        for(size_type i = 0; i < length; i++) {
          buffer[i] = static_cast<Buffer::value_type>(*src++);
        }

        return length;
      }

      /*
       * Floating-Point numbers according to C99:
       *
       * floating-constant:
       *   decimal-floating-constant
       *   hexadecimal-floating-constant   <- out-of-scope!
       *
       * decimal-floating-constant:
       *   fractional-constant exponent-part-OPT floating-suffix-OPT
       *   digit-sequence exponent-part floating-suffix-OPT
       *
       * fractional-constant:
       *   digit-sequence-OPT '.' digit-sequence
       *   digit-sequence '.'
       */
      inline static bool isRealPrefix(const StringView& input)
      {
        for(size_type pos = 0; pos < input.size(); pos++) {
          const char_type ch = input[pos];

          if( isDigit(ch) ) {
            continue;

          } else if( ch == g::dot ) {
            /*
             * NOTE: Covers the cases:
             *   digit-sequence-OPT '.' digit-sequence
             *   digit-sequence '.'
             */
            return true;

          } else if( (ch == g::e  ||  ch == g::E)  &&  pos > 0 ) {
            /*
             * NOTE: Covers the case:
             *   digit-sequence exponent-part
             */
            return true;

          } else {
            break;
          }
        }

        return false;
      }

      inline static Range makeRange(Buffer& buffer,
                                    const StringView& input,
                                    const size_type prefix)
      {
        if constexpr( sizeof(char_type) == sizeof(Buffer::value_type) ) {
          const char *first = reinterpret_cast<const char*>(input.data()) + prefix;
          const char  *last = first + input.size() - prefix;

          return Range{first, last};
        }

        const size_type length = fillBuffer(buffer, input, prefix);

        return Range{buffer.data(), buffer.data() + length};
      }

      inline static size_type maxLength(const StringView& input,
                                        const size_type prefix)
      {
        return std::min(Buffer().size(), input.size() - prefix);
      }

      inline static BasePrefix scanBasePrefix(const StringView& input,
                                              const bool have_prefix)
      {
        constexpr int BASE_DEC = 10;
        constexpr int BASE_HEX = 16;
        constexpr int BASE_OCT =  8;

        constexpr size_type ZERO = 0;
        constexpr size_type  ONE = 1;
        constexpr size_type  TWO = 2;

        if( have_prefix ) {
          if(        input.size() > TWO  &&  input[0] == g::zero  &&
                     (input[1] == g::x  ||  input[1] == g::X)  &&
                     isHexDigit(input[2]) ) {
            return BasePrefix{BASE_HEX, TWO};
          } else if( input.size() > ONE  &&  input[0] == g::zero  &&
                     isOctDigit(input[1]) ) {
            return BasePrefix{BASE_OCT, ONE};
          }
        } // Prefix

        if( input.size() > ZERO  &&  isDigit(input[0]) ) {
          return BasePrefix{BASE_DEC, ZERO};
        }

        return BasePrefix{0, 0};
      }
    };

  } // namespace impl_lexer

} // namespace cs
