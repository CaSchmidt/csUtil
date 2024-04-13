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

#include <cs/Lexer/Scanner.h>
#include <cs/Lexer/TokenUtil.h>

namespace cs {

  template<typename CharT>
  class CharLiteralScanner : public IScanner<CharT> {
  private:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    using typename IScanner<CharT>::StringView;
    using typename IScanner<CharT>::size_type;
    using typename IScanner<CharT>::value_type;

    using String = std::basic_string<value_type>;

    CharLiteralScanner(const String& charset,
                       const ctor_tag& = ctor_tag()) noexcept
      : _charset(charset)
    {
    }

    ~CharLiteralScanner() noexcept
    {
    }

    TokenPtr scan(const StringView& input) const
    {
      // (1) Sanity Check ////////////////////////////////////////////////////

      if( input.empty() ) {
        return TokenPtr();
      }

      // (2) input[0] part of set? ///////////////////////////////////////////

      const value_type ch = input[0];
      if( _charset.find_first_of(ch) == String::npos ) {
        return TokenPtr();
      }

      return Token::make_literal(ch);
    }

    static ScannerPtr<value_type> make(const String& charset)
    {
      return std::make_unique<CharLiteralScanner<value_type>>(charset);
    }

  private:
    String _charset;
  };

} // namespace cs
