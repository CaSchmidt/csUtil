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

#include <array>
#include <charconv>
#include <tuple>

#include <cs/Core/CharUtil.h>
#include <cs/Lexer/Scanner.h>
#include <cs/Lexer/ValueToken.h>

namespace cs {

  template<typename CharT, typename T>
  class CIntegralScanner : public IScanner<CharT> {
  private:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    using typename IScanner<CharT>::StringView;
    using typename IScanner<CharT>::size_type;
    using typename IScanner<CharT>::value_type;

    using intergral_type = T;

    CIntegralScanner(const tokenid_t id, const bool prefix,
                     const ctor_tag& = ctor_tag()) noexcept
      : _id{id}
      , _prefix{prefix}
    {
    }

    ~CIntegralScanner() noexcept
    {
    }

    TokenPtr scan(const StringView& input) const
    {
      // (1) Sanity Check ////////////////////////////////////////////////////

      if( input.empty() ) {
        return TokenPtr();
      }

      // (2) Scan base & prefix //////////////////////////////////////////////

      const auto [base, prefix] = scanBasePrefix(input);
      {}

      // (3) Collect Consecutive Digits //////////////////////////////////////

      Buffer buffer;
      const size_type numDigits = base == 8
          ? scanDigits(buffer, input, prefix, lambda_is_oct<value_type>())
          : base == 16
            ? scanDigits(buffer, input, prefix, lambda_is_hex<value_type>())
            : scanDigits(buffer, input, prefix, lambda_is_digit<value_type>());

      if( numDigits == 0 ) {
        return TokenPtr();
      }

      // (4) Conversion //////////////////////////////////////////////////////

      intergral_type value{0};
      const std::from_chars_result result =
          std::from_chars(buffer.data(), buffer.data() + numDigits, value, base);
      if( result.ec != std::errc{} ) {
        return TokenPtr();
      }

      // Done! ///////////////////////////////////////////////////////////////

      return ValueToken<intergral_type>::make(_id, value, prefix + numDigits);
    }

    static ScannerPtr<value_type> make(const tokenid_t id, const bool prefix = false)
    {
      return std::make_unique<CIntegralScanner<value_type,intergral_type>>(id, prefix);
    }

  private:
    using BasePrefix = std::tuple<int,size_type>;
    using Buffer = std::array<char,128>;

    inline BasePrefix scanBasePrefix(const StringView& input) const
    {
      using g = glyph<value_type>;

      constexpr int BASE_DEC = 10;
      constexpr int BASE_HEX = 16;
      constexpr int BASE_OCT =  8;

      constexpr size_type ZERO = 0;
      constexpr size_type  ONE = 1;
      constexpr size_type  TWO = 2;

      if( _prefix ) {
        if(        input.size() > TWO  &&  input[0] == g::zero  &&
                   (input[1] == g::x  ||  input[1] == g::X) ) {
          return std::make_tuple(BASE_HEX, TWO);
        } else if( input.size() > ONE  &&  input[0] == g::zero ) {
          return std::make_tuple(BASE_OCT, ONE);
        }
      } // Prefix

      return std::make_tuple(BASE_DEC, ZERO);
    }

    template<typename UnaryPred>
    inline size_type scanDigits(Buffer& buffer, const StringView& input,
                                const size_type prefix, UnaryPred is_digit) const
    {
      size_type pos = prefix;
      for(size_type i = 0; i < buffer.size()  &&  pos < input.size(); i++, pos++) {
        const value_type ch = input[pos];

        if( is_digit(ch) ) {
          buffer[i] = static_cast<Buffer::value_type>(ch);
        } else {
          break;
        }
      }

      return pos - prefix;
    }

    tokenid_t _id{0};
    bool      _prefix{false};
  };

} // namespace cs
