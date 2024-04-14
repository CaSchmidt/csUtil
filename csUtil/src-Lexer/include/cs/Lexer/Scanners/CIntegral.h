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

#include <cs/Lexer/ConvWrapperImpl.h>
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
    using typename IScanner<CharT>::char_type;

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
      using Conv = impl_lexer::ConvWrapper<CharT>;

      const auto [value, consumed] = Conv::template toValue<intergral_type>(input, _prefix);
      {}
      if( consumed == 0 ) {
        return TokenPtr();
      }

      return ValueToken<intergral_type>::make(_id, value, consumed);
    }

    static ScannerPtr<char_type> make(const tokenid_t id, const bool prefix = false)
    {
      return std::make_unique<CIntegralScanner<char_type,intergral_type>>(id, prefix);
    }

  private:
    tokenid_t _id{0};
    bool      _prefix{false};
  };

} // namespace cs
